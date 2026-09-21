# Comprehensive Architectural Plan: Chrome Offline Dino for Kindle Keyboard (K3 / K3G / K3W)

## 1. Context & Motivation
The objective is to implement a high-performance, offline, standalone Chrome Dino game tailored specifically to the physical, electrical, and hardware realities of the Amazon Kindle Keyboard (Kindle 3 / K3, K3G, K3W; Freescale i.MX353 ARM1136JF-S @ 532MHz, 256MB RAM, 600×800 16-level grayscale E-Ink Pearl display).

Standard browser-based games (running at 60 FPS in WebKit) are completely unplayable on E-Ink due to hardware refresh latency (~100–450ms) and catastrophic ghosting. This project provides a native, zero-dependency C++17 implementation that directly drives the i.MX35 Electronic Paper Display Controller (EPDC) via `/dev/fb0` and `/dev/input/event0,1,2`. It adheres strictly to Object Calisthenics, SOLID principles, fail-fast mechanics, and robust defensive programming.

---

## 2. Deep Hardware Specifications & Physical Limitations

### 2.1 SoC, Memory, and Thermal Constraints
- **Processor:** Freescale i.MX353 (ARMv6TEJ core ARM1136JF-S with VFPv2, 532 MHz).
  - *Constraint:* Modest single-core compute, no multi-threading overhead desired. Float operations are softfp.
  - *Design:* Integer and fixed-point math for all physics (jump velocity, gravity, obstacle movement, hitbox coordinates). Zero floating-point calculations in the hot game loop.
- **RAM:** 256 MB Mobile DDR SDRAM shared between kernel, framebuffers, and user applications.
  - *Constraint:* Stock Kindle Java framework (`/etc/init.d/framework`) consumes 120–150MB of RAM and runs active background daemons (`lipc-daemon`, `powerd`, `webreaderd`).
  - *Design:* Launcher script stops the Java framework (`/etc/init.d/framework stop`) to reclaim >120MB RAM and isolate CPU time. The C++ game has a static footprint of < 2MB user RAM (two 600×800 8bpp canvas buffers = ~960KB; fixed-capacity object pools; zero dynamic heap allocations in the game loop).
- **Thermal & Battery:** Passively cooled, 1750 mAh Li-ion cell.
  - *Constraint:* CPU spinloops cause rapid thermal throttling and battery drain.
  - *Design:* Event loop strictly sleeps via `poll()` with millisecond timeout matched to the 80ms simulation deadline. Zero busy-waiting.

### 2.2 E-Ink Pearl Display Mechanics & Controller (`mxc_epdc_fb`)
- **Panel:** 6.0-inch E-Ink Pearl, 600×800 resolution (native portrait), 167 ppi, 16-level grayscale (4 bits per pixel).
- **Framebuffer (`/dev/fb0`):**
  - Packed 4bpp: 2 pixels per byte $\rightarrow 600 \times 800 / 2 = 240,000$ bytes total.
  - Byte nibble layout: Left pixel (even X) in high nibble `bits[7:4]`, right pixel (odd X) in low nibble `bits[3:0]`.
  - Polarity: Inverted Kindle hardware polarity (`0x0` = pure White, `0xF` = pure Black).
- **E-Ink Refresh Latency & Waveforms:**
  - **DU (Direct Update / partial):** ~100ms - 150ms physical transition time. Fast 1-bit/monochrome update without full-screen flashing. However, repeated DU updates over the same area leave electrostatic ghosting (faint shadows/burn-in).
  - **GC16 (Grayscale Clear 16 / full):** ~400ms - 500ms transition time. Pulses through inverse phases (white $\rightarrow$ black $\rightarrow$ white flash) to completely cancel trapped electrostatic charge and erase all ghosting.
- **Ioctl Interface (`0x46dd` - `FBIO_EINK_UPDATE_DISPLAY_AREA`):**
  - Requires `struct update_area_t`:
    ```c
    struct update_area_t {
        int x1, y1;     // Top-left
        int x2, y2;     // Bottom-right (CRITICAL: exclusive coordinates x2 = x1 + width, y2 = y1 + height)
        fx_type which_fx; // 0 = fx_update_partial (DU), 1 = fx_update_full (GC16)
        __u8 *buffer;   // NULL = read directly from mmap framebuffer
    };
    ```

### 2.3 Ghosting Mitigation & Refresh Strategy
In an endless runner, naive partial refreshes quickly degrade the screen into an unreadable smear of ghost trails. Full-screen GC16 flashes during running gameplay would blind the player for 450ms and cause unfair collisions. The design solves this with a **Three-Tier Refresh Hierarchy**:
1. **Padded Dirty-Rect DU Partial Refresh (Active Gameplay):**
   - The game tracks the union bounding box of old and new entity positions (Dino, cacti, pterodactyls, ground changes, HUD score).
   - Dirty rectangles are expanded outward by a 2-pixel margin. This ensures the trailing edges receive a pure "white" write pulse, erasing the physical ghost edge of moving sprites.
   - Dirty regions are merged to avoid hardware EPDC collision errors. Max 2–3 update regions per frame.
2. **Milestone Milestone Pause + GC16 Flash (Every 700 points / Day-Night Inversion):**
   - At score milestones (700, 1400, 2100...), the game inverts palette (Day: white bg/black sprites $\leftrightarrow$ Night: black bg/white sprites).
   - To prevent mid-run deaths during the 450ms GC16 flash:
     - The game enters a brief **Milestone Freeze** (~500ms): physics and obstacle generation freeze.
     - A full GC16 flash is triggered, erasing all accumulated ghosting.
     - Normal 12.5 FPS gameplay resumes seamlessly once the screen is crisp.
3. **Event-Driven GC16 Flashes:**
   - Initial application startup.
   - Collision / Game Over state (screen freezes and does a clean GC16 flash).
   - Game Restart (wiping the Game Over overlay and resetting the world).
   - Clean shutdown back to Kindle OS.

### 2.4 Keyboard, 5-Way Nav, and Page Button Constraints
- **Physical Key Ergonomics:**
  - Kindle Keyboard chiclet keys are small, stiff membrane switches. Continuous tapping on Spacebar fatigues the player quickly.
  - The side **Page Turn Buttons** (`<` and `>`) on the left and right bezels rest naturally under the player's thumbs when holding the device like a classic portable console.
- **Dual/Unified Control Scheme:**
  - **Jump:**
    - Spacebar (`KEY_SPACE` on `/dev/input/event0`).
    - Up Arrow / 5-Way Up (`KEY_UP` on `/dev/input/event0` or `/dev/input/event1`).
    - 5-Way Center Click (`KEY_ENTER` / code `194`).
    - **Top / Prev Page Buttons** (`193` or `109` on `/dev/input/event2`).
  - **Duck:**
    - Down Arrow / 5-Way Down (`KEY_DOWN` on `/dev/input/event0` or `/dev/input/event1`).
    - 'D' key on keyboard.
    - **Bottom / Next Page Buttons** (`104` or `191` on `/dev/input/event2`).
  - **Quit:**
    - Back button (`158` / `KEY_BACK`) or Escape (`KEY_ESC`).
    - 'Q' key.
- **Input Edge Cases & Debouncing:**
  - **Key Repeat Suppression for Jump:** Linux evdev fires `value=1` (press), `value=2` (autorepeat), `value=0` (release). Jump requests are strictly **edge-triggered** on `value=1`. Autorepeat (`value=2`) is discarded so holding Space/PageUp will not trigger consecutive jumps mid-air or immediately on landing.
  - **Held State for Duck:** Ducking is a **level-sensitive state**. `value=1` enters duck; `value=2` maintains duck; only `value=0` exits duck. Releasing while under a pterodactyl causes an immediate collision if the standing hitbox intersects.
  - **Input Multiplexing & Grabbing:**
    - Uses `poll()` to multiplex `/dev/input/event0`, `/dev/input/event1`, and `/dev/input/event2`.
    - Applies `ioctl(fd, EVIOCGRAB, 1)` on startup to prevent background daemons or lipc scripts from capturing or misinterpreting keystrokes, and releases grab cleanly (`EVIOCGRAB, 0`) on shutdown.

### 2.5 Power Management, Sleep & OS Daemons
- **`powerd` Daemon:** Monitors user inactivity and forces sleep/screensaver after 10 minutes.
  - *Mitigation:* Launcher script sets `lipc-set-prop com.lab126.powerd -i deferSuspend 1`.
  - In addition, the game can poke `deferSuspend` periodically or on keypresses.
- **Framework Lifecyle & Clean Exit:**
  - When the user presses 'Q' or Back, the application terminates with code 0.
  - Shell `trap` in `launch_kindle.sh` unconditionally restarts `/etc/init.d/framework start` and clears the framebuffer, ensuring the Kindle e-reader UI returns cleanly without requiring a reboot.
- **Filesystem Permissions (`/mnt/us`):**
  - `/mnt/us` is formatted with VFAT/FAT32 and mounted with `noexec` on stock Kindle 3 firmwares.
  - *Packaging Strategy:* Binary is placed in `/mnt/us/dino/bin/dino`. If executed on a system where `/mnt/us` has `noexec`, the launcher copies or symlinks the binary to `/tmp/dino` or `/var/local/dino` before execution, ensuring reliable startup across all jailbreak variants.
  - **High Score Persistence:** Atomic write via temporary file (`highscore.tmp` $\rightarrow$ `fsync` $\rightarrow$ `rename` to `highscore.dat`) prevents corrupted FAT file tables if the device battery dies mid-write.

---

## 3. Game Mechanics & Tuning for 12.5 FPS E-Ink

### 3.1 Physics & Timing Constants
At 12.5 FPS (80ms per frame), physics cannot use standard 60 FPS numbers:
- **Base Horizontal Speed:** 8 fixed-point pixels/frame (~100 px/sec across 600px screen $\rightarrow$ obstacle travels across screen in ~6 seconds).
- **Speed Acceleration:** Increases by 1 fixed-point unit every 100 points, capping at 18 pixels/frame.
- **Jump Physics:**
  - Jump impulse: Initial vertical velocity $V_y = -22$ pixels/frame.
  - Gravity: $G = +3$ pixels/frame$^2$.
  - Total jump duration: ~14 frames (~1.12 seconds), reaching peak height of ~80 pixels above ground.
  - Gives the player sufficient reaction window even with ~100ms e-ink display latency!
- **Hitboxes (Fairness with Margin):**
  - Player Standing: $44 \times 48$ px (Hitbox inset by 3px on all sides so visual bumps don't cause frustrating deaths).
  - Player Ducking: $56 \times 28$ px (Reduced height allows sliding cleanly under high pterodactyls).
  - Small Cactus: $20 \times 36$ px.
  - Large Cactus / Cactus Group: $32 \times 48$ px.
  - Pterodactyl: $42 \times 32$ px (flies at 2 distinct heights: high = duck to pass; low = jump to clear).

---

## 4. Software Architecture & File Layout

```text
/home/samuelcaldas/repos/kindle/dino/
├── Makefile
├── README.md
├── scripts/
│   ├── launch_kindle.sh
│   ├── package_kindle.sh
│   └── check_limits.sh
├── src/
│   ├── main.cpp                        # Composition root & signal handling
│   ├── application/
│   │   ├── application.h / .cpp        # 12.5 FPS fixed-step loop & phase coordinator
│   │   ├── application_options.h / .cpp# Command line flags (--ansi, --ppm, --seed)
│   │   └── hardware_context.h / .cpp   # Factory for native Kindle vs host fallbacks
│   ├── domain/
│   │   ├── geometry.h / .cpp           # ScreenCoordinate, BoundingBox (exclusive bounds)
│   │   ├── game_values.h / .cpp        # FixedPoint, Velocity, ScoreValue, DistanceValue
│   │   ├── player.h / .cpp             # Dino state machine (running, jumping, ducking, dead)
│   │   ├── obstacle.h / .cpp           # Cactus & Pterodactyl representations
│   │   ├── obstacle_collection.h / .cpp# Static array first-class collection (max 16)
│   │   ├── cloud.h / .cpp              # Parallax cloud
│   │   ├── cloud_collection.h / .cpp   # First-class cloud pool (max 6)
│   │   ├── ground.h / .cpp             # Procedural ground strip with bumps
│   │   ├── score.h / .cpp              # Score ticker, milestone tracking
│   │   ├── theme.h / .cpp              # Day/Night palette & milestone state
│   │   ├── random_generator.h / .cpp   # Deterministic 32-bit LCG
│   │   └── game_world.h / .cpp         # World aggregate
│   ├── game/
│   │   ├── input_action.h / .cpp       # JumpPressed, DuckHeld, DuckReleased, Quit
│   │   ├── input_state.h / .cpp        # Edge-trigger & level-trigger tracker
│   │   ├── game_rules.h / .cpp         # Constants: speeds, gravity, milestones
│   │   ├── collision_detector.h / .cpp # Inset AABB collision testing
│   │   ├── obstacle_spawner.h / .cpp   # Procedural spawner with min gap enforcement
│   │   ├── game_snapshot.h / .cpp      # Immutable state snapshot for rendering
│   │   └── game_engine.h / .cpp        # Step function: input -> physics -> collision -> state
│   ├── graphics/
│   │   ├── canvas.h / .cpp             # 600x800 8bpp double-buffered drawing surface
│   │   ├── bitmap_font.h / .cpp        # Compact embedded 5x7 bitmap font
│   │   ├── sprite_renderer.h / .cpp    # Procedural pixel art (dino, cacti, pterodactyl)
│   │   ├── scene_renderer.h / .cpp     # Composites snapshot onto canvas
│   │   ├── dirty_region_tracker.h / .cpp# Merges bounding boxes with 2px ghosting margin
│   │   ├── frame_differ.h / .cpp       # Compares front/back buffers for damage
│   │   └── refresh_coordinator.h / .cpp# Decides DU vs GC16 and milestone freeze
│   ├── hal/
│   │   ├── frame_buffer.h              # Abstract framebuffer interface
│   │   ├── frame_buffer_fb0.h / .cpp   # /dev/fb0 mmap, 4bpp packing & polarity inversion
│   │   ├── packed_gray.h / .cpp        # Pure 8bpp -> 4bpp nibble packer
│   │   ├── eink_controller.h           # Abstract e-ink interface
│   │   ├── eink_controller_mxc.h / .cpp# ioctl 0x46dd implementation
│   │   ├── input_device.h              # Abstract input device interface
│   │   ├── input_device_evdev.h / .cpp # event0/1/2 non-blocking poll with EVIOCGRAB
│   │   └── fallback_devices.h / .cpp   # Host ANSI terminal runner & PPM snapshotter
│   ├── persistence/
│   │   ├── high_score_store.h          # Abstract store interface
│   │   ├── file_high_score_store.h / .cpp # POSIX atomic file store
│   │   └── storage_path.h / .cpp       # Path resolver (/mnt/us -> $HOME -> ./)
│   └── util/
│       ├── monotonic_clock.h / .cpp    # clock_gettime(CLOCK_MONOTONIC) wrapper
│       ├── signal_guard.h / .cpp       # sig_atomic_t shutdown flag
│       └── debug_log.h                 # Zero-cost compile-time logging
└── tests/
    ├── test_framework.h                # Zero-dependency test runner
    ├── test_runner.cpp                 # Main test executable
    ├── mocks/                          # Mock devices & clocks
    ├── test_geometry.cpp
    ├── test_player.cpp
    ├── test_obstacles.cpp
    ├── test_spawner.cpp
    ├── test_collision.cpp
    ├── test_score_and_theme.cpp
    ├── test_high_score.cpp
    ├── test_canvas.cpp
    ├── test_frame_differ.cpp
    ├── test_refresh_coordinator.cpp
    ├── test_packed_gray.cpp
    ├── test_input_mapping.cpp
    └── test_game_engine.cpp
```

---

## 5. Build, Packaging & Verification

### 5.1 Build Targets
- `make host`: Native binary `bin/dino` for development.
- `make test`: Runs all unit and integration tests.
- `make asan`: Builds host with AddressSanitizer and UndefinedBehaviorSanitizer.
- `make run-ansi`: Runs playable downsampled ASCII/ANSI game in terminal.
- `make kindle`: Cross-compiles for ARMv6 softfp (`bin/dino-arm32`) using `armv6-linux-musleabi-g++` or `arm-linux-gnueabi-g++`:
  `-std=c++17 -march=armv6j -mtune=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -Wl,--gc-sections`.
- `make package`: Builds `dino-k3-arm32.tar.gz` ready for USB deployment to `/mnt/us/dino`.

### 5.2 Verification Checklist
1. **Packed Gray Tests**: Verifies 4bpp nibbles, polarity inversion (`0x0` white, `0xF` black), even/odd column order, and exact 240,000-byte output.
2. **Physics & Collision Tests**: Verifies jump curve, gravity at 12.5 FPS, duck hitbox reduction, and clearance over cacti and under pterodactyls.
3. **Ghosting & Waveform Tests**: Verifies DU partial update bounding box expansion (+2px margin), box merging, and correct GC16 triggering on 700-point milestones, Game Over, and startup.
4. **Input Tests**: Verifies edge-triggered jump (ignoring autorepeat), level-triggered duck, and key mapping across keyboard, 5-way D-pad, and side page buttons.
5. **Persistence Tests**: Verifies atomic write, parse validation, corrupted file handling, and path fallbacks.
6. **Host Smoke Run**: Runs 50 frames in PPM mode (`bin/dino --ppm --frames 50`) and tests interactive play via ANSI terminal mode.
