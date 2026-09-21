# Chrome Offline Dino for Kindle Keyboard (K3 / K3G / K3W)

A high-performance, offline, standalone Chrome Dino game tailored specifically to the physical, electrical, and hardware realities of the Amazon Kindle Keyboard (Kindle 3 / K3, K3G, K3W; Freescale i.MX353 ARM1136JF-S @ 532MHz, 256MB RAM, 600×800 16-level grayscale E-Ink Pearl display).

Built in native C++17 with zero third-party dependencies, adhering strictly to **Object Calisthenics**, **SOLID principles**, and fail-fast defensive programming.

---

## 1. Hardware Architecture & Display Driver

Standard 60 FPS browser engines are unplayable on E-Ink due to high hardware refresh latencies (~100–450ms) and electrostatic ghosting. This implementation directly interfaces with the Linux kernel drivers:

- **Framebuffer (`/dev/fb0`):**
  - Packed 4bpp nibble encoding: 2 pixels per byte ($600 \times 800 / 2 = 240,000$ bytes total).
  - Byte nibble layout: Left pixel (even X) in bits `[7:4]`, right pixel (odd X) in bits `[3:0]`.
  - Inverted hardware polarity: `0x0` = pure White, `0xF` = pure Black.
- **EPDC Waveform Controller (`mxc_epdc_fb`):**
  - Uses `ioctl(fd, 0x46dd, &update_area)` (`FBIO_EINK_UPDATE_DISPLAY_AREA`) with exclusive bounds (`x2 = x1 + width`, `y2 = y1 + height`).
- **Memory Footprint:**
  - Static footprint < 2MB user RAM (two 600×800 8bpp canvas buffers = ~960KB; fixed-capacity object pools; zero dynamic heap allocations in the game loop).

---

## 2. Three-Tier Refresh Hierarchy & Anti-Ghosting

1. **Padded Dirty-Rect DU Partial Refresh (Active Gameplay):**
   - Active runner runs at a fixed **12.5 FPS (80ms fixed step)** using Direct Update (DU ~100ms) mode.
   - Damaged bounding boxes are expanded outward by a **2-pixel margin**, forcing trailing edges to receive an explicit white pulse to erase moving sprite ghost artifacts.
   - Merges adjacent regions to maintain a maximum of 2–3 update regions per frame.
2. **Milestone Milestone Pause + GC16 Flash (Every 700 points / Day-Night Inversion):**
   - Every 700 points, the palette inverts (Day $\leftrightarrow$ Night).
   - The engine triggers a brief **Milestone Freeze** (~500ms / 6 frames) while performing a full-screen Grayscale Clear 16 (GC16 ~450ms) flash, wiping all accumulated charge without causing unfair mid-air player deaths.
3. **Event-Driven GC16 Full Refreshes:**
   - Startup, Collision / Game Over, Game Restart, and clean exit back to Kindle OS.

---

## 3. Ergonomic Dual Control Scheme

- **Jump (Edge-Triggered, ignores autorepeat):**
  - Spacebar (`KEY_SPACE`)
  - 5-Way Up Arrow (`KEY_UP`)
  - 5-Way Center Click (`KEY_ENTER` / `194`)
  - **Top Page Turn Buttons** (`<` buttons on left/right bezels: `193` or `109`)
- **Duck (Level-Sensitive held state):**
  - Down Arrow (`KEY_DOWN`)
  - 'D' Key
  - **Bottom Page Turn Buttons** (`>` buttons on left/right bezels: `104` or `191`)
- **Restart (on Game Over):**
  - Spacebar or 5-Way Up
- **Quit:**
  - Back button (`KEY_BACK` / `158`), Escape (`KEY_ESC`), or 'Q' Key

---

## 4. Build Targets

| Target | Description |
|---|---|
| `make host` | Builds native desktop development binary `bin/dino`. |
| `make test` | Builds and runs all 35 unit/integration tests with zero dependencies. |
| `make asan` | Builds and runs test suite under AddressSanitizer and UndefinedBehaviorSanitizer. |
| `make run-ansi` | Launches the game in the terminal using downsampled ANSI block graphics. |
| `make kindle` | Cross-compiles `bin/dino-arm32` for ARMv6 softfp (`arm-linux-gnueabi-g++`). |
| `make package` | Builds release tarball `bin/dino-k3-arm32.tar.gz` for USB installation. |
| `make clean` | Cleans object files and temporary outputs. |

---

## 5. Deployment & Execution on Kindle Keyboard

1. Connect Kindle Keyboard to your computer via USB.
2. Extract or copy the `dino` directory from `bin/dino-k3-arm32.tar.gz` to `/mnt/us/dino/`.
3. Safely unmount/eject the Kindle USB drive.
4. SSH into the Kindle or open a shell, then run:
   ```sh
   /mnt/us/dino/launch.sh
   ```
5. `launch.sh` temporarily stops the Java framework (`/etc/init.d/framework stop`) to free >120MB RAM, disables sleep timer (`deferSuspend`), executes the game, and automatically restores the Kindle e-reader UI on exit.
