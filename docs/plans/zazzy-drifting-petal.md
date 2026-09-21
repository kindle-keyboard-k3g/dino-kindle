# Plan: GitHub README & MIT License Polish for Dino Kindle

## 1. Context & Objectives
The repository `kindle-keyboard-k3g/dino-kindle` contains a native C++17 offline Chrome Dino game engineered for the Amazon Kindle Keyboard (Kindle 3 / K3, K3G, K3W).

The goal of this task is to:
1. Add an official **MIT License** (`LICENSE`) for the project (Copyright (c) 2026 Samuel Caldas and contributors), matching sibling projects like `papergram`.
2. Overhaul `README.md` into a best-in-class GitHub repository document adhering to modern open-source conventions:
   - Engaging project header with visual metadata badges (License, C++ standard, target platform, ARMv6 softfp architecture, test coverage, zero external dependencies).
   - High-contrast ASCII art screen layout depicting the Kindle E-Ink Pearl gameplay interface (Dino, cacti, pterodactyl, parallax clouds, ground strip, and retro-digital HUD).
   - Interactive Table of Contents with working anchor links.
   - Highlights & Key Engineering Features (zero heap allocations in game loop, 12.5 FPS fixed-step physics, three-tier anti-ghosting waveform control, direct `/dev/fb0` and `/dev/input` access).
   - Comprehensive Hardware Specifications & Compatibility matrix.
   - ASCII System Architecture diagram detailing the engine, graphics pipeline, HAL drivers, persistence, and signal management.
   - Physical Keyboard & Navigation Reference table covering keyboard keys, 5-way D-pad, and ergonomic bezel page-turn buttons.
   - Toolchain setup, cross-compilation instructions (`arm-linux-gnueabi-g++`), local host emulation (`make run-ansi` and PPM mode), and sanitizers (`make asan`).
   - Device deployment, USB installation, and Kindle OS framework lifecycle execution.
   - Object Calisthenics & Coding Standards breakdown.
   - Contributing guidelines and License & Attribution section.

---

## 2. Key Files to Create / Modify
- **`LICENSE`** (New): Standard MIT License text with copyright 2026 Samuel Caldas and contributors.
- **`README.md`** (Updated): Complete rewrite aligning with GitHub best practices and `papergram` styling.

---

## 3. Detailed Structure of Improved README.md

### 3.1 Title & Badges
- `# Dino Kindle 🦖⚡`
- Subtitle: `> **High-performance, offline Chrome Dino runner crafted in modern C++17 for the Amazon Kindle Keyboard (Kindle 3 / K3G / K3W) E-Ink Pearl display.**`
- Badges:
  - `[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)`
  - `[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B17)`
  - `[![Platform](https://img.shields.io/badge/Platform-Kindle%20Keyboard%20(K3)-black?logo=amazon)](https://en.wikipedia.org/wiki/Amazon_Kindle#Kindle_Keyboard)`
  - `[![Architecture](https://img.shields.io/badge/Arch-ARMv6%20%7C%20softfp-orange)](#hardware-specifications--compatibility)`
  - `[![Tests](https://img.shields.io/badge/Tests-35%20passed%20%7C%20ASan%20clean-brightgreen)](#quickstart--local-host-emulation)`
  - `[![Dependencies](https://img.shields.io/badge/Dependencies-Zero%20External-success)](#highlights--key-features)`

### 3.2 ASCII Screen & Interface
Render an accurate ASCII art representation of the 600×800 Kindle display:
- Parallax sky clouds
- Running/jumping Dino with leg movement
- Low and high obstacles (small/large cacti, flying pterodactyls)
- Ground terrain strip with procedural bumps
- HUD showing `HI 01240  00482`, Day/Night mode indicator, and control hints.

### 3.3 Table of Contents
Anchor links matching GitHub markdown slug generation:
- Highlights & Key Features
- ASCII Screen & Interface
- Three-Tier Refresh Hierarchy & Anti-Ghosting
- Hardware Specifications & Compatibility
- System Architecture
- Physical Controls & Ergonomics
- Prerequisites & Toolchain Setup
- Quickstart & Local Host Emulation
- Kindle Deployment & Running
- Clean Code & Object Calisthenics
- Troubleshooting & FAQ
- License & Attribution

### 3.4 Highlights & Key Features
- **100% Offline & Native**: Runs standalone on device; zero browser overhead, zero Node.js/Java dependencies.
- **Sub-100ms E-Ink Waveform Engine**: Direct `/dev/fb0` mmap and ioctl `0x46dd` (`FBIO_EINK_UPDATE_DISPLAY_AREA`) DU partial updates.
- **Three-Tier Ghosting Prevention**: 2px padded dirty rects, 700-point milestone freeze with GC16 flash, event-driven clears.
- **Ergonomic Bezel Controls**: Native support for side page-turn buttons (`<` / `>`) enabling comfortable handheld gaming.
- **Power-Fail-Safe Persistence**: Atomic `fsync` + `rename` score store.
- **Ultra-Lean Resource Budget**: < 2MB RAM footprint, binary < 100 KB.

### 3.5 Hardware Specifications Table
Matrix comparing Kindle Keyboard reality vs Dino architecture:
- Processor (Freescale i.MX353 ARM1136JF-S @ 532MHz, ARMv6 softfp)
- Memory (256MB RAM, static < 2MB footprint)
- Display (6.0" E-Ink Pearl 600×800 4bpp packed nibbles, inverted polarity)
- Input Devices (38-key keyboard, 5-way D-pad, side page rockers)
- Storage & OS (Internal flash, Linux 2.6 kernel, Kindle OS 3.x)

### 3.6 System Architecture Diagram
ASCII diagram illustrating data flow and modular layers:
- Application Event Loop (`poll()` multiplexing evdev + monotonic clock)
- Game Engine (`GameWorld`, `PlayerPhysics`, `CollisionDetector`, `ObstacleSpawner`)
- Graphics Pipeline (`SceneRenderer`, `Canvas`, `SpriteRenderer`, `DirtyRegionTracker`, `FrameDiffer`, `RefreshCoordinator`)
- Hardware Abstraction Layer (`FrameBufferFb0`, `EinkControllerMxc`, `InputDeviceEvdev`, `FallbackDevices`)
- Persistence Layer (`FileHighScoreStore`, `StoragePath`)

### 3.7 Physical Controls Table
Clear mapping of action, keyboard key, 5-way D-pad, and bezel page-turn buttons:
- Jump (Space, 5-Way Up, 5-Way Center, Top Page Turn `<`)
- Duck (Down Arrow, 'D' Key, Bottom Page Turn `>`)
- Restart (Space, 5-Way Up)
- Quit (Back Button, Esc, 'Q' Key)

### 3.8 Build & Toolchain Guide
- Host build commands (`make host`, `make test`, `make asan`, `make run-ansi`)
- Cross-compilation for Kindle (`make kindle`, `make package`)
- Step-by-step USB installation to `/mnt/us/dino/` and SSH launch via `launch.sh`

### 3.9 License Section
- References the MIT License with a link to `LICENSE`.

---

## 4. Verification Plan
1. Validate `LICENSE` exists and is formatted correctly.
2. Verify all internal anchor links in `README.md` resolve to existing headers.
3. Verify that `make test` still builds and passes 35/35 tests cleanly.
4. Verify `./scripts/check_limits.sh` succeeds.
5. Review `git diff` for formatting and clarity.
