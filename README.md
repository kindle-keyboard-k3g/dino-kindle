# Dino Kindle 🦖⚡

> **High-performance, offline Chrome Dino runner crafted in modern C++17 for the Amazon Kindle Keyboard (Kindle 3 / K3G / K3W) E-Ink Pearl display.**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Platform](https://img.shields.io/badge/Platform-Kindle%20Keyboard%20(K3)-black?logo=amazon)](https://en.wikipedia.org/wiki/Amazon_Kindle#Kindle_Keyboard)
[![Architecture](https://img.shields.io/badge/Arch-ARMv6%20%7C%20softfp-orange)](#hardware-specifications--compatibility)
[![Tests](https://img.shields.io/badge/Tests-35%20passed%20%7C%20ASan%20clean-brightgreen)](#quickstart--local-host-emulation)
[![Dependencies](https://img.shields.io/badge/Dependencies-Zero%20External-success)](#highlights--key-features)

---

## ASCII Screen & Interface

Dino Kindle renders directly to the Kindle Keyboard's 600×800 16-level grayscale E-Ink display via `/dev/fb0` with packed 4bpp nibble encoding, procedural parallax clouds, jumping physics, and responsive bezel controls:

```text
+-------------------------------------------------------------------------+
|                                           HI 01420   00388   [DAY MODE] |
|                                                                         |
|            .---.                                                        |
|           (  .  )                                    .---.              |
|          (   .   )                                  (  .  )             |
|           `-----'                                    `-----'            |
|                                                                         |
|                                                                         |
|                                                    /\_/\                |
|                                                   ( o.o )>              |
|                                                    /   \   [PTERODACTYL]|
|                   .-.                                                   |
|                  /   \                                                  |
|                 |     |                                                 |
|                 |     |---.                                             |
|            __   |     |   |                                             |
|           / _)  |  _  |   |                                             |
|    .-^^^-/ /    | | | |   |                                             |
| __/       /     | |_| |   |                                             |
| <__.|_|-|_|     |_| |_|___|                                             |
+=========================================================================+
| .   ..     .  _   ...  .    ..   . _ .   ...   .  _  .   ..  .    ..  . |
+-------------------------------------------------------------------------+
| [Space / 5-Way Up / Top Bezel (<)] JUMP   |   [Down / Bottom (>)] DUCK  |
+-------------------------------------------------------------------------+
```

---

## Table of Contents

- [Highlights & Key Features](#highlights--key-features)
- [ASCII Screen & Interface](#ascii-screen--interface)
- [Three-Tier Refresh Hierarchy & Anti-Ghosting](#three-tier-refresh-hierarchy--anti-ghosting)
- [Hardware Specifications & Compatibility](#hardware-specifications--compatibility)
- [System Architecture](#system-architecture)
- [Physical Controls & Ergonomics](#physical-controls--ergonomics)
- [Prerequisites & Toolchain Setup](#prerequisites--toolchain-setup)
- [Quickstart & Local Host Emulation](#quickstart--local-host-emulation)
- [Kindle Deployment & Running](#kindle-deployment--running)
- [Clean Code & Object Calisthenics](#clean-code--object-calisthenics)
- [Troubleshooting & FAQ](#troubleshooting--faq)
- [Contributing & Development](#contributing--development)
- [License & Attribution](#license--attribution)

---

## Highlights & Key Features

- **100% Native & Serverless**: Built from scratch in pure C++17 with zero third-party dependencies. No WebKit, no Node.js, and no Java VM overhead.
- **Sub-100ms E-Ink Waveform Engine**: Direct memory-mapped Linux framebuffer access (`/dev/fb0`, 240 KB packed 4bpp nibbles) paired with low-latency EPDC Direct Update (DU) partial refreshes via `ioctl(0x46dd)` (`FBIO_EINK_UPDATE_DISPLAY_AREA`).
- **Three-Tier Ghosting Elimination**:
  - **Padded Dirty Rectangles**: Real-time damage tracking expanded outward by a **2-pixel margin**, forcing trailing sprite edges to receive white erasing pulses.
  - **Milestone Freeze & GC16 Clear**: Every 700 points, triggers a palette swap (Day $\leftrightarrow$ Night) accompanied by a 500ms simulation freeze and full Grayscale Clear 16 (GC16 ~450ms) flash.
  - **Event-Driven Flashes**: Seamless GC16 sweeps on startup, collision/game over, and restart.
- **Ergonomic Dual Bezel Controls**: Complete support for the physical QWERTY keyboard, 5-way D-pad, and the side **Page Turn buttons** (`<` and `>`), turning the Kindle into a natural, fatigue-free handheld gaming console.
- **Power-Fail-Safe Persistence**: High scores are written atomically using POSIX `fsync` followed by `rename` (`highscore.tmp` $\to$ `highscore.dat`), completely protecting FAT32 flash tables against abrupt battery death.
- **Minimal Resource Footprint**: Entire static memory consumption is under 2 MB user RAM; compiled ARM32 binary is under 100 KB.

---

## Three-Tier Refresh Hierarchy & Anti-Ghosting

Standard 60 FPS animations turn electronic paper into an illegible smear of dark toner particles. Dino Kindle solves this with a three-tier hardware waveform hierarchy:

| Refresh Mode | Waveform | Latency | Visual Effect | Triggers |
| :--- | :--- | :--- | :--- | :--- |
| **Active Gameplay** | `fx_update_partial` (DU) | ~100 ms | 1-bit fast partial update | Frame-to-frame sprite motion with +2px padded bounding boxes |
| **Milestone Flash** | `fx_update_full` (GC16) | ~450 ms | Full-screen inverse flash | Every 700 points (Day $\leftrightarrow$ Night inversion, 500ms freeze) |
| **Event Clear** | `fx_update_full` (GC16) | ~450 ms | Charge neutralization | Game launch, game over crash, game restart, and clean exit |

---

## Hardware Specifications & Compatibility

| Component | Kindle Keyboard (K3 / K3G / K3W) Reality | Dino Kindle Architecture |
| :--- | :--- | :--- |
| **SoC / CPU** | Freescale i.MX353, ARM1136JF-S @ 532 MHz (ARMv6TEJ) | Native ARMv6 softfp binary (`-march=armv6j -mtune=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp`); pure 16.16 fixed-point math with zero floating-point operations in the hot loop. |
| **RAM** | 256 MB Mobile DDR SDRAM (~100 MB free) | Peak RAM < 2 MB; launcher temporarily halts stock Java framework (`/etc/init.d/framework stop`) to reclaim >120 MB and eliminate background CPU jitter. |
| **Display** | 6.0" E-Ink Pearl, 600×800, 16-level grayscale (4bpp) | Packed 4bpp nibbles ($600 \times 800 / 2 = 240,000$ bytes total); inverted hardware polarity (`0x0` = pure White, `0xF` = pure Black). |
| **Input** | Physical 38-key QWERTY, 5-way D-pad, side page rockers | Linux evdev multiplexer polling `/dev/input/event[0,1,2]` with `EVIOCGRAB 1` to prevent background OS interception; edge-triggered jump debouncing. |
| **Storage** | 4 GB internal flash formatted as FAT32 (`/mnt/us`) | Deployed to `/mnt/us/dino/`; launcher executes from `/tmp/dino` to seamlessly bypass VFAT `noexec` restrictions across all jailbreaks. |
| **Supported Models** | Kindle Keyboard 3G (D00901), Wi-Fi (B008), 3G UK (B00A) | Compatible with Amazon Kindle 3.x firmwares (glibc 2.5+ embedded). |

---

## System Architecture

```text
+-------------------------------------------------------------------------+
|                        Application Event Loop                           |
|       (12.5 FPS / 80ms Fixed-Step Simulation: poll() + Monotonic Clock) |
+-------------------+---------------------------------+-------------------+
                    |                                 |
+-------------------v---------------+   +-------------v-------------------+
|          Graphics Engine          |   |           Game Engine           |
| - Canvas (Double Buffer 600x800)  |   | - GameWorld & EngineEntities    |
| - BitmapFont (5x7 Embedded Font)  |   | - PlayerPhysics (16.16 FixedPt) |
| - SpriteRenderer (Packed Bitmaps) |   | - ObstacleCollection (Max 16)   |
| - DirtyRegionTracker (+2px Margin)|   | - ObstacleSpawner (Procedural)  |
| - FrameDiffer (Damage Detection)  |   | - CollisionDetector (Inset AABB)|
| - RefreshCoordinator (DU vs GC16) |   | - Score & Milestone Theme       |
+-------------------+---------------+   +---------------------------------+
                    |                                 |
+-------------------v---------------+   +-------------v-------------------+
|    Hardware Abstraction (HAL)     |   |        Persistence Layer        |
| - FrameBufferFb0 (/dev/fb0 mmap)  |   | - FileHighScoreStore            |
| - EinkControllerMxc (ioctl 0x46dd)|   | - StoragePath (/mnt/us fallback)|
| - InputDeviceEvdev (event0,1,2)   |   | - Atomic fsync -> rename        |
| - FallbackDevices (ANSI / PPM)    |   +---------------------------------+
+-----------------------------------+
```

---

## Physical Controls & Ergonomics

Dino Kindle provides a unified control layout tailored for the physical ergonomics of the Kindle Keyboard chassis:

| Action | Keyboard Key | 5-Way D-Pad | Side Bezel Rockers | Evdev Codes |
| :--- | :--- | :--- | :--- | :--- |
| **Jump** (Edge-triggered) | `Spacebar` | `Up Arrow` | `Center Click` | **Top Page Turn** (`<`) | `KEY_SPACE` (57), `KEY_UP` (103), `194`, `193`, `109` |
| **Duck** (Level-sensitive) | `'D'` Key | `Down Arrow` | — | **Bottom Page Turn** (`>`) | `KEY_D` (32), `KEY_DOWN` (108), `104`, `191` |
| **Restart** (Game Over) | `Spacebar` | `Up Arrow` | `Center Click` | **Top Page Turn** (`<`) | `KEY_SPACE` (57), `KEY_UP` (103), `194` |
| **Quit to Kindle OS** | `'Q'` Key | `Back Button` | `Escape` | — | `KEY_Q` (16), `KEY_BACK` (158), `KEY_ESC` (1) |

*Note: Jump requests ignore evdev auto-repeat events (`value=2`), ensuring holding the jump button never triggers accidental consecutive jumps upon landing.*

---

## Prerequisites & Toolchain Setup

### Host Development (Linux / macOS)

- **C++17 Compiler**: `g++` (≥ 9.0) or `clang++` (≥ 10.0)
- **Build Automation**: GNU `make`
- **Sanitizers (Optional)**: `libasan` and `libubsan` for memory safety checks

### Cross-Compilation for Kindle Keyboard (ARMv6 softfp)

On Debian / Ubuntu systems, install the ARM cross-compiler:

```bash
sudo apt-get update
sudo apt-get install -y gcc-arm-linux-gnueabi g++-arm-linux-gnueabi
```

---

## Quickstart & Local Host Emulation

You can develop, test, and play Dino Kindle directly on your desktop machine without hardware:

### 1. Build and Run the Interactive ANSI Terminal Game

```bash
make run-ansi
```

*Controls: `Space` = Jump, `D` / `Down Arrow` = Duck, `Q` = Quit.*

### 2. Run the Unit & Integration Test Suite

```bash
make test
```

Executes all 35 test cases covering fixed-point physics, collision hitboxes, waveform triggers, input debouncing, dirty rectangle calculation, and atomic persistence.

### 3. Run Sanitizers (AddressSanitizer & UndefinedBehaviorSanitizer)

```bash
make asan
```

### 4. Export Headless Frame Snapshots (PPM Format)

```bash
make host
./bin/dino --ppm --frames 50
```

Exports frame renders directly to PPM image files for graphical inspection.

---

## Kindle Deployment & Running

### Step 1: Package the Release Archive

```bash
make package
```

Generates the self-contained deployment bundle: `bin/dino-k3-arm32.tar.gz`.

### Step 2: Install via USB

1. Connect your Kindle Keyboard to your computer using a standard Micro-USB cable.
2. Extract the `dino` folder from `bin/dino-k3-arm32.tar.gz` directly into the root of the Kindle user partition:
   ```text
   /mnt/us/dino/
   ├── bin/
   │   └── dino
   ├── launch.sh
   └── README.txt
   ```
3. Safely unmount/eject the Kindle USB volume.

### Step 3: Launch the Game

Open an SSH or terminal session on your Kindle Keyboard and run:

```sh
/mnt/us/dino/launch.sh
```

### What `launch.sh` Does Automatically:
- Suspends system power timeouts (`lipc-set-prop com.lab126.powerd -i deferSuspend 1`).
- Temporarily halts the heavy Java e-reader framework (`/etc/init.d/framework stop`), freeing >120 MB RAM and dedicating 100% CPU to the game.
- Copies the binary to `/tmp/dino` to transparently bypass FAT32 `noexec` mounts.
- Catches exit signals (`trap cleanup EXIT INT TERM`) to guarantee the Kindle Java UI and home screen are cleanly restored upon exit.

---

## Clean Code & Object Calisthenics

The codebase strictly complies with the following engineering standards:

- **Zero `else` Statements**: All branching logic utilizes early returns, guard clauses, or polymorphism.
- **Single Indentation Level**: Methods maintain one level of indentation.
- **Strict Size Limits**: Functions $\le 15$ lines, classes $\le 100$ lines, and modules $< 200$ lines.
- **Instance Variable Bounds**: $\le 2$ instance variables per class via focused compositional grouping (`PlayerPhysics`, `PlayerAnimation`, `ActorGroup`, `EnvironmentGroup`, `DisplayBundle`, `ApplicationCore`).
- **First-Class Collections**: Dynamic entities are managed via typed first-class collections (`ObstacleCollection`, `CloudCollection`) with zero dynamic heap allocations during gameplay.
- **Fail-Fast Boundary Validation**: Robust validation at system boundaries and atomic file operations.

Audit limits at any time using:

```bash
./scripts/check_limits.sh
```

---

## Troubleshooting & FAQ

**Q: Screen appears faint or has ghosting artifacts after prolonged play.**  
A: Dino Kindle automatically performs a full-screen GC16 clearing flash every 700 points during the day/night palette swap, as well as on every game over. If manual clearance is needed, restarting or quitting and relaunching triggers a clean waveform flash.

**Q: The game does not start from `/mnt/us`.**  
A: Kindle firmwares often mount user storage (`/mnt/us`) with the `noexec` flag. Always launch using `/mnt/us/dino/launch.sh`, which automatically copies the binary to `/tmp/dino` before execution.

**Q: How can I compile directly on the Kindle device?**  
A: You can use Tiny C Compiler for Kindle available in the [`kindle-keyboard-k3g/kindle-tiny-c-compiler`](https://github.com/kindle-keyboard-k3g/kindle-tiny-c-compiler) repository.

---

## Contributing & Development

Contributions and PRs are welcome! When submitting patches:

1. Ensure all 35 tests pass: `make test`
2. Ensure memory sanitizers pass: `make asan`
3. Audit Object Calisthenics limits: `./scripts/check_limits.sh`
4. Verify ARM cross-compilation: `make kindle`

---

## License & Attribution

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

Developed with ❤️ for the Kindle Keyboard homebrew and preservation community.
