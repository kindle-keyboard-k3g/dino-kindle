# Dino Kindle Documentation

Welcome to the internal engineering documentation for **Dino Kindle**, the high-performance offline Chrome Dino runner crafted in modern C++17 for the Amazon Kindle Keyboard (Kindle 3 / K3G / K3W) E-Ink Pearl display.

---

## Documentation Index

| Document | Description |
| :--- | :--- |
| [**Architecture Overview**](architecture.md) | High-level system design, hexagonal / layered architecture, data flow, Clean Code and Object Calisthenics rules. |
| [**Hardware & E-Ink Subsystem**](hardware-and-eink.md) | Freescale i.MX353 SoC, 4bpp packed framebuffer (`/dev/fb0`), EPDC waveform controller (`ioctl 0x46dd`), input evdev multiplexer, and anti-ghosting hierarchy. |
| [**Graphics Pipeline & Sprites**](graphics-and-assets.md) | 1-bit packed MSB-first sprite bitmasks, procedural scrolling ground bumps, Canvas drawing engine, damage differ, and dirty region tracking. |
| [**Game Mechanics & Physics**](game-mechanics.md) | 16.16 fixed-point arithmetic, jump physics, collision detection with inset hitboxes, fair obstacle spawning, speed scaling, and day/night cycle. |
| [**Persistence Subsystem**](persistence.md) | Power-fail-safe high score storage, storage path discovery across VFAT flash and local storage, atomic write-fsync-rename pattern. |
| [**Testing & Toolchain Guide**](testing-and-toolchain.md) | Cross-compilation for ARMv6 softfp, host test framework, AddressSanitizer (ASan/UBSan), ANSI terminal emulator, debugging options (`--debug`, `--godmode`), and Kindle deployment. |

---

## Architecture at a Glance

The project follows strict separation of concerns, Object Calisthenics, and deterministic zero-allocation gameplay:

```text
+---------------------------------------------------------------+
|                       src/main.cpp                            |
|             (CLI Options & Application Lifecycle)             |
+---------------------------------------------------------------+
                               |
                               v
+---------------------------------------------------------------+
|                      Application Layer                        |
|   Application, HardwareContext, DisplayBundle, FrameLimiter   |
+---------------------------------------------------------------+
        |                                       |
        v                                       v
+-----------------------+               +-----------------------+
|      Game Layer       |               |    Graphics Layer     |
| GameEngine, Rules,    | ------------> | SceneRenderer, Canvas,|
| InputState, Spawner   | (GameSnapshot)| FrameDiffer, Sprites  |
+-----------------------+               +-----------------------+
        |                                       |
        v                                       v
+-----------------------+               +-----------------------+
|     Domain Layer      |               |       HAL Layer       |
| Player, Obstacles,    |               | FrameBufferFb0,       |
| Ground, Cloud, Score, |               | EinkControllerMxc,    |
| Geometry, FixedPoint  |               | InputDeviceEvdev      |
+-----------------------+               +-----------------------+
        |                                       |
        +-------------------+-------------------+
                            |
                            v
+---------------------------------------------------------------+
|                   Persistence & Util Layers                   |
|   FileHighScoreStore, StoragePath, MonotonicClock, SignalGuard|
+---------------------------------------------------------------+
```

---

## Architectural Principles & Constraints

1. **Deterministic 16.16 Fixed-Point Math**: Absolutely zero floating-point operations in simulation or collision code, running on hardware with software floating-point (`softfp`).
2. **Zero Dynamic Allocation During Gameplay**: Entities, obstacles, and clouds reside in fixed-capacity first-class collections; canvas and framebuffers are pre-allocated at startup.
3. **Strict Clean Code & Object Calisthenics**:
   - Every file strictly `< 200` lines of code.
   - Every function `≤ 15` lines.
   - Every class `≤ 100` lines with `≤ 2` instance variables.
   - Zero `else` keywords in domain and game logic.
4. **Three-Tier Waveform Hierarchy**: Direct Update (DU ~100ms) for fast 12.5 FPS gameplay with 2-pixel margin damage padding; Grayscale Clear (GC16 ~450ms) for milestones, collisions, and startup.
