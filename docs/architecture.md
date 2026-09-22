# System Architecture

## 1. Architectural Overview

Dino Kindle is designed around a clean, layered architectural model inspired by hexagonal and clean architecture patterns. The core domain and simulation logic remain completely isolated from hardware specifics, graphics rendering, and OS dependencies.

```text
[ Hardware / Peripherals ] <---> [ HAL Layer ]
                                       ^
                                       |
[ Application Layer ] ---------> [ Graphics Layer ]
        |                              ^
        v                              | (via GameSnapshot)
  [ Game Layer ] ----------------------+
        |
        v
 [ Domain Layer ] <------------- [ Persistence Layer ]
```

---

## 2. Layer Decomposition

### 2.1 Domain Layer (`src/domain/`)
The foundational domain model represents the Chrome Dino runner world with pure entities and value types.
- **Independence**: Zero dependencies on Linux headers, POSIX syscalls, standard I/O streams, or rendering libraries.
- **Value Objects**:
  - `FixedPoint`: 16.16 signed fixed-point arithmetic (`FRACTIONAL_BITS = 16`, `ONE_RAW = 65536`).
  - `ScreenCoordinate`: 2D discrete integer screen coordinates $(x, y)$.
  - `Dimensions`: 2D discrete width and height.
  - `BoundingBox`: 2D axis-aligned bounding box with exclusive upper bounds ($x_2 = x + \text{width}$, $y_2 = y + \text{height}$) and spatial operations (`intersects`, `expanded`, `inset`, `merged_with`, `clamped_to`).
  - `ScoreValue` & `DistanceValue`: Strongly-typed numeric primitives tracking points and distance.
  - `ColorValue`: Hardware grayscale palette enum (`White = 0x0`, `LightGray = 0x5`, `DarkGray = 0xA`, `Black = 0xF`).
- **Entities & Aggregates**:
  - `Player`: Manages Dino state, composed of `PlayerPhysics` (jump impulse, gravity) and `PlayerAnimation` (pose and leg toggling).
  - `Obstacle`: Cactus (Small, Large) and Pterodactyl (Low, High) with precise visual boxes and inset hitboxes.
  - `Ground`: Procedural ground line with scroll offset.
  - `Cloud`: Parallax drifting clouds.
  - `Theme`: Manages day/night palette inversion and milestone flash flags.
  - `Score`: Aggregates `ScoreBoard` and `DistanceTracker` to trigger milestones every 700 points.
  - `GameWorld`: Root aggregate holding `ActorGroup` (Player + ObstacleCollection) and `EnvironmentGroup` (Clouds + Ground).
- **First-Class Collections**:
  - `ObstacleCollection`: Fixed-capacity array (`MAX_CAPACITY = 16`) storing active obstacles without heap allocation.
  - `CloudCollection`: Fixed-capacity array (`MAX_CAPACITY = 6`) storing drifting clouds.

### 2.2 Game Layer (`src/game/`)
Implements game rules, state transitions, physics progression, and input mapping.
- `GameEngine`: The step coordinator. Each 80ms tick executes:
  1. Poll and apply input actions (Jump, Duck, Restart, Quit).
  2. Compute speed based on current score via `GameRules::calculate_speed`.
  3. Advance physics (Player jump trajectory, obstacle/cloud motion, ground scrolling).
  4. Perform collision detection using `CollisionDetector::has_collision`.
  5. Check score milestone triggers for theme inversion.
  6. Produce an immutable `GameSnapshot`.
- `GameRules`: Encapsulates constants and scaling formulas:
  - Base speed: 8 pixels/frame; maximum speed: 18 pixels/frame.
  - Target frame interval: 80 ms (12.5 FPS).
  - Milestone interval: 700 points.
  - Obstacle spawn gap: 220 to 450 pixels.
- `ObstacleSpawner`: Deterministic procedural generator using `RandomGenerator` (LCG) to place cacti and pterodactyls while enforcing fairness gaps.
- `InputState`: Edge-triggered momentary triggers (Jump, Restart) and level-triggered states (Ducking, Quit).
- `CollisionDetector`: Tests spatial intersections between player's inset hitbox and obstacle hitboxes.

### 2.3 Graphics Layer (`src/graphics/`)
Handles rasterization and dirty region tracking into an 8bpp monochrome raster buffer.
- `Canvas`: 600×800 pixel buffer supporting clipping, pixel writes, solid rectangle fills, and 1-bit bitmap blitting.
- `SpriteData` & `UiSprites`: Compile-time 1-bit MSB-first packed bitmasks for all Dino poses, obstacles, HUD glyphs, Game Over banner, Restart button, and 44 procedural ground bumps.
- `SpriteRenderer`: Blits entities and ground bumps onto the canvas.
- `SceneRenderer`: Composites entire scene from a `GameSnapshot` (Environment $\to$ Actors $\to$ HUD $\to$ Game Over overlay).
- `FrameDiffer`: Diffing algorithm comparing front and back canvas buffers to find the minimal enclosing damaged bounding box.
- `DirtyRegionTracker`: Manages damage regions and expands them by an anti-ghosting margin (+2px).
- `RefreshCoordinator`: State machine deciding between `PartialDirectUpdate` (DU ~100ms) and `FullGrayscaleClear` (GC16 ~450ms), managing a 500ms simulation freeze on score milestones.

### 2.4 Hardware Abstraction Layer (`src/hal/`)
Isolates hardware interaction behind abstract interfaces:
- `FrameBuffer`: Interface for presenting pixels.
  - `FrameBufferFb0`: Memory-maps `/dev/fb0` on Kindle hardware and writes packed 4bpp nibbles.
  - `AnsiTerminalFrameBuffer`: Local terminal fallback downsampling canvas to ANSI text blocks.
  - `PpmFrameBuffer`: Headless export saving frames as binary `.ppm` files.
- `EinkController`: Interface for triggering E-Ink waveform updates.
  - `EinkControllerMxc`: Issues `ioctl(0x46dd)` (`FBIO_EINK_UPDATE_DISPLAY_AREA`) to the Freescale EPDC driver.
  - `MockEinkController`: Verification mock for tests.
- `InputDevice`: Interface for polling input events.
  - `InputDeviceEvdev`: Non-blocking Linux evdev multiplexer polling `/dev/input/event0`, `event1`, and `event2`.
  - `AnsiTerminalInput`: Raw non-canonical POSIX terminal keyboard reader.
- `PackedGray`: High-speed conversion utility packing two 8bpp canvas pixels into a single 4bpp byte.

### 2.5 Persistence Layer (`src/persistence/`)
- `HighScoreStore`: Abstract interface for loading and storing high scores.
- `FileHighScoreStore`: Atomic implementation writing to temporary file, executing POSIX `fsync`, and atomically replacing destination via `rename`.
- `StoragePath`: Heuristic directory resolver selecting `/mnt/us/dino` (Kindle USB user partition), `$HOME/.dino`, or `./` current directory.

### 2.6 Application Layer (`src/application/`)
- `ApplicationOptions`: Command-line parser (`--ansi`, `--ppm`, `--seed`, `--frames`).
- `HardwareContext`: Factory creating matching `DisplayBundle` and `PeripheralBundle` according to options.
- `Application`: Master game loop executing fixed 80ms ticks using `util::MonotonicClock` and handling POSIX signals via `util::SignalGuard`.

---

## 3. Clean Code & Object Calisthenics Enforcement

The codebase strictly enforces the following Object Calisthenics and Clean Code rules across all modules:

1. **One Indentation Level Per Method**: Complex nested loops or conditions are broken into small helper functions.
2. **No `else` Keyword**: All conditional branching relies on early guard returns or polymorphism.
3. **Small Entities**:
   - Every file strictly `< 200` lines.
   - Every function `≤ 15` lines.
   - Every class `≤ 100` lines.
4. **Maximum 2 Instance Variables Per Class**: Classes with multiple responsibilities are decomposed into cohesive focused objects (e.g. `Player` decomposes into `PlayerPhysics` and `PlayerAnimation`; `ApplicationCore` decomposes into `EngineBundle` and `PipelineBundle`).
5. **First-Class Collections**: Collections (`ObstacleCollection`, `CloudCollection`) wrap containers directly and expose only domain operations.
6. **Zero Allocation in Game Loop**: Entities and collections are statically sized or pre-allocated; no `new`, `malloc`, or dynamic vector resizing occurs during gameplay.

### Verification Script
Compliance is audited automatically via `./scripts/check_limits.sh`:
```bash
./scripts/check_limits.sh
# Auditing Clean Code & Object Calisthenics Limits in src...
# SUCCESS: All files strictly comply with the <200 lines limit!
```
