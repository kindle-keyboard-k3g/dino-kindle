# Graphics Pipeline & Sprites

## 1. Overview & Rendering Philosophy

Dino Kindle targets a 600×800 16-level grayscale E-Ink Pearl screen. Because electrophoretic microcapsules have physical latency (~100 ms for Direct Updates), rendering must minimize pixel churn and avoid unnecessary full-buffer repaints.

```text
[ GameSnapshot ]
       |
       v
[ SceneRenderer ]  --->  [ Canvas (8bpp) ]
                                |
                                v
                         [ FrameDiffer ]  --->  Minimal BoundingBox
                                                       |
                                                       v
                                            [ DirtyRegionTracker ] (+2px padding)
                                                       |
                                                       v
                                               [ EinkController ] (DU ioctl)
```

The pipeline uses an internal 8bpp single-byte-per-pixel working canvas (`graphics::Canvas`), which is downsampled and packed into 4bpp nibbles (`hal::PackedGray`) only when copying dirty regions to the Linux framebuffer (`/dev/fb0`).

---

## 2. Authentic 1-Bit Sprite Bitmasks

All visual elements are encoded as compile-time, 1-bit MSB-first packed bitmasks (`graphics::SpriteData` and `graphics::UiSprites`). Each byte contains 8 consecutive horizontal pixels:
- Bit value `1`: Foreground (draw pixel with current foreground color, e.g. Black in Day mode).
- Bit value `0`: Transparent (preserves background color).

### 2.1 Dinosaur Sprites (`SpriteData`)
- **Standing / Jump**: 44 × 47 pixels.
- **Running (Left / Right Leg Frames)**: 44 × 47 pixels, alternating legs based on distance traveled.
- **Ducking (Left / Right Leg Frames)**: 55 × 26 pixels, reduced vertical hitbox.
- **Crashed (Game Over)**: 44 × 47 pixels, featuring the distinctive 'X' eye.

### 2.2 Obstacle Sprites (`SpriteData`)
- **Small Cactus**:
  - Single: 17 × 35 pixels.
  - Double / Group: 34 × 35 pixels.
- **Large Cactus**:
  - Single: 25 × 50 pixels.
  - Double / Group: 50 × 50 pixels.
- **Pterodactyl**:
  - Wing Up: 46 × 40 pixels.
  - Wing Down: 46 × 40 pixels.

### 2.3 User Interface & HUD Sprites (`UiSprites`)
- **"GAME OVER" Banner**: 191 × 11 pixels authentic Chromium pixel-art graphic banner.
- **Circular Restart Icon**: 36 × 32 pixels authentic circular reload arrow button.
- **Digital Score Digits**: 10 × 13 pixels per digit (`0` through `9`), providing tall, crisp retro numbers.
- **High Score Prefix ("HI")**: 20 × 13 pixels.
- **Cloud**: 46 × 14 pixels.

---

## 3. Canvas & Software Rasterizer (`Canvas`)

The `Canvas` class manages a statically pre-allocated 480,000-byte buffer ($600 \times 800 \times 1$ byte per pixel).

### 3.1 Clipping & Bounds Safety
Every drawing operation validates coordinates against screen bounds $[0, 599] \times [0, 799]$. Pixels outside the visible canvas are clipped immediately without branching inside inner loops:
```cpp
void Canvas::set_pixel(ScreenCoordinate coord, ColorValue color) {
    if (coord.x() >= 0 && coord.x() < WIDTH &&
        coord.y() >= 0 && coord.y() < HEIGHT) {
        buffer_[coord.y() * WIDTH + coord.x()] = color;
    }
}
```

### 3.2 1-Bit Sprite Blitting
When blitting 1-bit packed bitmasks, bytes are decoded row by row:
```cpp
void Canvas::draw_bitmap_1bpp(ScreenCoordinate origin, Dimensions dims,
                             const uint8_t* mask, ColorValue color) {
    const int32_t bytes_per_row = (dims.width() + 7) / 8;
    for (int32_t row = 0; row < dims.height(); ++row) {
        for (int32_t col = 0; col < dims.width(); ++col) {
            const uint8_t byte = mask[row * bytes_per_row + (col / 8)];
            if ((byte >> (7 - (col % 8))) & 0x01) {
                set_pixel(ScreenCoordinate(origin.x() + col, origin.y() + row), color);
            }
        }
    }
}
```

---

## 4. Ground Generation & Scrolling

The ground baseline is situated at $Y = 520$.

1. **Horizon Line**: A solid 1-pixel baseline rendered across the entire 600-pixel width.
2. **Procedural Ground Bumps**: Rather than rendering large static background textures, Dino Kindle uses 44 procedural ground bumps and star speckles indexed by world scroll position.
3. **Seamless Repeat**: As distance increases, the ground wrap modulo $(x - \text{offset}) \pmod{600}$ ensures seamless continuous scrolling with zero memory allocation.

---

## 5. Frame Diffing & Dirty Region Tracking

### 5.1 FrameDiffer (`graphics::FrameDiffer`)
Dino Kindle maintains two canvas buffers: `front` (current frame) and `back` (previous frame).
- `FrameDiffer::find_damage(front, back)` performs a scan comparing rows and columns.
- Returns a tight `BoundingBox` enclosing all modified pixels between the two frames.
- If no pixels changed, an empty box is returned and no hardware ioctl is issued.

### 5.2 DirtyRegionTracker (`graphics::DirtyRegionTracker`)
E-Ink pigment particles suffer from trailing ghost artifacts if only the exact changed pixels are refreshed. `DirtyRegionTracker`:
1. Expands the damage bounding box by `GHOSTING_MARGIN = 2` pixels in all directions.
2. Clamps the box to display boundaries $[0, 599] \times [0, 799]$.
3. Aligns horizontal bounds to 2-pixel boundaries (even columns) to facilitate 4bpp nibble packing.

---

## 6. Refresh Coordinator (`graphics::RefreshCoordinator`)

`RefreshCoordinator` determines which waveform is dispatched:
- **`PartialDirectUpdate`**: Dispatched during normal running frames (~100 ms DU). Only the bounding box calculated by `DirtyRegionTracker` is sent to the EPDC.
- **`FullGrayscaleClear`**: Dispatched on:
  - Startup initialization.
  - Game Over collision.
  - Restart.
  - Every 700-point milestone (coupled with a 500 ms gameplay freeze to allow full particle realignment).
