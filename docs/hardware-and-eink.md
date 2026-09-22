# Hardware & E-Ink Subsystem

## 1. Kindle Keyboard Hardware Specifications

The Amazon Kindle Keyboard (3rd Generation, model K3 / K3G / K3W, codename "Shasta") was released in 2010. Its embedded architecture imposes distinct operational constraints:

| Specification | Hardware Reality | Impact on Dino Kindle Design |
| :--- | :--- | :--- |
| **SoC** | Freescale i.MX353 (ARM1136JF-S core @ 532 MHz) | Low instruction throughput; instruction pipeline must avoid stalls and float emulation. |
| **Architecture** | ARMv6TEJ (`-march=armv6j -mtune=arm1136jf-s`) | Soft-float ABI (`-mfloat-abi=softfp`); floating-point math incurs software library calls. Physics uses 16.16 fixed-point math exclusively. |
| **RAM** | 256 MB Mobile DDR SDRAM | System framework consumes ~150 MB. Launcher halts Java framework (`/etc/init.d/framework stop`) to free >120 MB and eliminate JVM garbage collection pauses. Total game RAM is < 2 MB. |
| **Display Panel** | 6.0" E-Ink Pearl, 600×800 pixels | 16-level grayscale (4 bits per pixel). Electrophoretic display requires microsecond electrostatic pulses to shift pigment particles. |
| **Display Controller** | Integrated Freescale EPDC (Electrophoretic Display Controller) | Driven via Linux framebuffer driver `/dev/fb0` and proprietary `mxcfb` ioctl commands. |
| **Storage** | 4 GB eMMC (divided into rootfs and `/mnt/us` FAT32 user partition) | Flash memory wear and abrupt power loss require atomic `fsync` + `rename` persistence. |

---

## 2. Framebuffer Structure & 4bpp Nibble Packing

The Kindle's display driver `/dev/fb0` exposes a memory-mapped buffer of 240,000 bytes:
$$\text{Total Bytes} = \frac{600 \times 800 \text{ pixels}}{2 \text{ pixels per byte}} = 240,000 \text{ bytes}$$

### 2.1 Hardware Polarity
Unlike traditional RGB or desktop grayscale displays where 0 is black and 255 is white, the Kindle Pearl hardware polarity is inverted:
- `0x0`: Pure White (pigment capsules driven completely to the surface)
- `0x5`: Light Gray
- `0xA`: Dark Gray
- `0xF`: Pure Black

### 2.2 Nibble Packing Algorithm (`PackedGray`)
The internal rendering engine renders to an 8bpp single-byte-per-pixel canvas (`graphics::Canvas`). When transferring to `/dev/fb0`, `hal::PackedGray` packs adjacent horizontal pixels into a single byte:

```text
Pixel (X, Y)      [4-bit: 0x0..0xF] -> Upper Nibble (bits 7..4)
Pixel (X+1, Y)    [4-bit: 0x0..0xF] -> Lower Nibble (bits 3..0)

Packed Byte = ((LeftPixel & 0x0F) << 4) | (RightPixel & 0x0F)
```

For partial region updates, `PackedGray::pack_region` packs only the bounding box rows, ensuring that byte offsets align cleanly with 2-pixel boundaries:
```cpp
const int32_t start_col = (box.left() / 2) * 2;
const int32_t end_col = ((box.right() + 1) / 2) * 2;
```

---

## 3. Electrophoretic Display Controller (EPDC) & Waveforms

Electronic paper relies on microcapsules containing negatively charged black particles and positively charged white particles suspended in clear fluid. To refresh the screen, the EPDC applies voltage pulses known as **waveforms**.

### 3.1 EPDC Ioctl Interface (`EinkControllerMxc`)
The Kindle kernel driver exposes the update ioctl `FBIO_EINK_UPDATE_DISPLAY_AREA` (`0x46dd`):

```cpp
struct mxcfb_rect {
    uint32_t top;
    uint32_t left;
    uint32_t width;
    uint32_t height;
};

struct mxcfb_update_data {
    struct mxcfb_rect update_region;
    uint32_t waveform_mode;
    uint32_t update_mode;
    uint32_t update_marker;
    int temp;
    unsigned int flags;
    struct mxcfb_alt_buffer_data alt_buffer_data;
};
```

### 3.2 Waveform Modes
Dino Kindle uses two distinct EPDC waveforms:

1. **Direct Update (DU - `WAVEFORM_MODE_DU = 1`, `UPDATE_MODE_PARTIAL = 0`)**:
   - **Latency**: ~100 ms.
   - **Behavior**: Fast 1-bit black/white transition without intermediate inversion pulses.
   - **Use Case**: Used during active 12.5 FPS gameplay frames. Only the damaged region (Dino, obstacles, ground bumps, score) is refreshed.
2. **Grayscale Clear 16 (GC16 - `WAVEFORM_MODE_GC16 = 2`, `UPDATE_MODE_FULL = 1`)**:
   - **Latency**: ~450 ms.
   - **Behavior**: Multi-stage electro-optical clearing sequence that drives particles through black and white states to completely neutralize residual electrostatic charges.
   - **Use Case**: Used during startup, game-over collision, game restart, and every 700-point milestone inversion.

---

## 4. Three-Tier Anti-Ghosting Strategy

Because DU updates leave faint residual ghost images from moving sprites, Dino Kindle implements a three-tier anti-ghosting architecture:

```text
Tier 1: Continuous Padded Dirty Rectangles (+2px Margin)
        [ Erases trailing sprite edges every single frame ]
                           |
                           v
Tier 2: Score Milestone Freeze & Palette Swap (Every 700 Points)
        [ 500ms Pause + Full GC16 Inversion Flash ]
                           |
                           v
Tier 3: Event-Driven Full Screen Clear
        [ Clean GC16 Sweep on Launch, Crash, and Restart ]
```

### 4.1 Tier 1: Padded Dirty Rectangles
When `DirtyRegionTracker` registers damage from moving actors, it automatically expands the bounding box by `GHOSTING_MARGIN = 2` pixels in all four directions:
```cpp
BoundingBox expanded(int32_t margin) const {
    return BoundingBox(
        ScreenCoordinate(origin_.x() - margin, origin_.y() - margin),
        Dimensions(dimensions_.width() + margin * 2, dimensions_.height() + margin * 2));
}
```
This guarantees that pixels vacated by Dino or an obstacle receive a pure white pulse to neutralize trailing toner remnants.

### 4.2 Tier 2: Milestone Freeze & GC16 Flashes
Every 700 points, the Chrome Dino game inverts colors (Day mode $\leftrightarrow$ Night mode). In Dino Kindle:
1. `GameEngine` detects `score.is_at_milestone()`.
2. `RefreshCoordinator::notify_milestone()` triggers a **6-frame freeze** (~500 ms at 80 ms/frame).
3. The game simulation pauses while the display controller dispatches a full GC16 clearing waveform.
4. The background inverts cleanly with zero ghosting before normal gameplay resumes.

### 4.3 Tier 3: Event-Driven Flashes
A full-screen GC16 refresh is dispatched unconditionally on:
- **Application Startup**: Clears the stock Kindle user interface.
- **Game Over (Collision)**: Cleans the frame to present the authentic Game Over banner and Restart icon.
- **Restart**: Prepares a pristine white canvas for the new run.
- **Application Exit**: Leaves the screen clean when returning to shell or the framework.

---

## 5. Linux Input Subsystem (`InputDeviceEvdev`)

The Kindle Keyboard features a rich set of physical controls mapped to Linux input event character devices:
- `/dev/input/event0`: Five-way navigation pad and select button.
- `/dev/input/event1`: 38-key physical QWERTY keyboard.
- `/dev/input/event2`: Left and right side Page Turn rocker buttons.

### 5.1 Device Grabbing (`EVIOCGRAB`)
To prevent the background Kindle OS or window manager from intercepting keystrokes (such as Home or Search keys), `InputDeviceEvdev` takes exclusive ownership of the input descriptors via `ioctl(fd, EVIOCGRAB, 1)`.

### 5.2 Key Mappings

| Action | Physical Key | Linux Input Keycode | Behavior |
| :--- | :--- | :--- | :--- |
| **Jump** | `Spacebar` | `KEY_SPACE (57)` | Edge-triggered jump impulse |
| **Jump** | `5-Way UP` | `KEY_UP (103)` | Edge-triggered jump impulse |
| **Jump** | `Top Page Turn (<)` | `KEY_PAGEUP (104)` | Ergonomic left/right hand thumb jump |
| **Duck** | `5-Way DOWN` | `KEY_DOWN (108)` | Level-triggered (held) duck pose |
| **Duck** | `Bottom Page Turn (>)` | `KEY_PAGEDOWN (109)` | Level-triggered thumb duck |
| **Restart** | `Spacebar` / `Up` | `KEY_SPACE` / `KEY_UP` | Edge-triggered restart on Game Over |
| **Quit** | `Backspace` / `Del` | `KEY_BACKSPACE (14)` | Graceful application exit |
| **Quit** | `'Q'` Key | `KEY_Q (16)` | Graceful application exit |
