# Testing & Toolchain Guide

## 1. Toolchain & Cross-Compilation

### 1.1 Target Architecture: Freescale i.MX353
The Amazon Kindle Keyboard (K3) runs an ARMv6 architecture core with soft-float ABI:
- **Processor**: ARM1136JF-S @ 532 MHz
- **Architecture Flags**: `-march=armv6j -mtune=arm1136jf-s`
- **Float ABI**: `-mfloat-abi=softfp`
- **C++ Standard**: Modern C++17 (`-std=c++17`)

### 1.2 Cross-Compiler Toolchains
Two main cross-toolchains are supported:
- **arm-linux-gnueabi-g++**: Standard GNU ARM toolchain on Debian/Ubuntu (`sudo apt-get install g++-arm-linux-gnueabi`).
- **kindle-toolchain / armv6j-cross**: Custom Kindle GCC toolchain providing pre-linked C runtimes matching Kindle OS 3.x eglibc.

### 1.3 Compilation Commands
```bash
# Build native desktop host binary (for local development, tests, and emulation)
make host

# Build debug binary with ASan/UBSan, -g -O0, and diagnostic logging
make debug

# Build cross-compiled ARM binary for Kindle 3
make kindle
```

---

## 2. Desktop Emulation, Debugging & Visual Verification

Dino Kindle provides multiple ways to test and visualize the game on a standard development workstation without transferring binaries to a physical Kindle:

### 2.1 ANSI Terminal Emulator (`--ansi`)
The built-in ANSI terminal emulator maps the 600×800 canvas down to standard terminal character cells using half-block characters (`▀` / `▄`):
```bash
make run-ansi
# Or directly:
./bin/dino --ansi
```
- **Controls**:
  - `Space` / `W` / `Up Arrow`: Jump
  - `S` / `Down Arrow`: Duck
  - `Q` / `Escape`: Quit
  - `Space` / `R`: Restart on Game Over

### 2.2 Debug Flags & Diagnostics (`--debug`, `--godmode`)
The game supports dedicated runtime debugging flags for diagnosis and collision tuning:
- **`--debug` / `-d`**:
  - Enables diagnostic logging to stderr via `util::DebugLog`.
  - Enables on-screen debug visual overlays: renders wireframe outlines around player and obstacle hitboxes using `Canvas::draw_rect_outline`, and prints `DBG: HITBOX ON`.
- **`--godmode` / `-g`**:
  - Enables invincibility: obstacle collisions are detected and logged, but do not trigger a Game Over state, allowing extended inspection of high-speed spawning patterns.

Example execution:
```bash
./bin/dino --ansi --debug --godmode
```

### 2.3 Headless PPM Frame Export (`--ppm`)
For pixel-perfect regression testing and visual inspection, `--ppm` exports rendered frames as binary portable pixmap (`.ppm`) image files:
```bash
./bin/dino --ppm --frames 10 --seed 42 --debug
```
This generates `output.ppm` or numbered frame files in the current working directory, which can be viewed with ImageMagick (`display`), `feh`, or VS Code.

---

## 3. Automated Test Suite

Dino Kindle includes a comprehensive unit testing framework covering domain logic, fixed-point math, collisions, graphics diffing, persistence, and debug options.

### 3.1 Running Unit Tests
```bash
make test
```
The test suite executes all unit test suites:
- `test_fixed_point`: Verifies 16.16 math, arithmetic limits, and conversions.
- `test_geometry`: Tests 2D coordinates, bounding box math, intersections, and insets.
- `test_player`: Verifies jump impulses, gravity integration, ducking states, and leg animation toggling.
- `test_obstacle`: Tests obstacle collection capacity, removal of off-screen actors, and spawner fairness.
- `test_collision`: Tests hitbox calculations and exact collision boundaries.
- `test_differ`: Verifies minimal dirty bounding box calculations and ghosting margin padding.
- `test_score`: Tests score accumulation and 700-point milestone triggers.
- `test_options`: Verifies CLI flag parsing (`--ansi`, `--ppm`, `--debug`, `--godmode`, `-d`, `-g`).
- `test_canvas`: Tests pixel operations, rectangle fill, and 1px wireframe outline drawing (`draw_rect_outline`).
- `test_game_engine`: Tests engine steps, collision handling, restart, and godmode bypass.
- `test_sprites`: Verifies sprite bitmasks, Game Over rendering, and debug hitbox overlays.
- `test_persistence`: Tests atomic write, read, and fallback handling.

### 3.2 AddressSanitizer & UndefinedBehaviorSanitizer
To guarantee memory safety and verify zero memory leaks or out-of-bounds array reads:
```bash
make asan
```
Compiles and runs all test suites with `-fsanitize=address,undefined -fno-omit-frame-pointer`.

---

## 4. Code Quality & Object Calisthenics Auditing

Compliance with Object Calisthenics and architecture limits is automated via `./scripts/check_limits.sh`:
```bash
./scripts/check_limits.sh
```

### Audited Rules:
1. **File Length**: Every source and header file must be strictly `< 200` lines.
2. **Function Length**: Functions must not exceed `15` lines.
3. **Class Length**: Classes must not exceed `100` lines.
4. **Member Variables**: Maximum of `2` member variables per class.
5. **No `else` Keyword**: Early returns, guard clauses, or polymorphism must be used instead.

---

## 5. Kindle Keyboard Deployment Guide

### 5.1 Prerequisites
1. Jailbroken Kindle Keyboard (Kindle 3) with USBNetwork and Launchpad or KUAL installed.
2. SSH or USB network connectivity (`192.168.15.244` / `192.168.2.2`).

### 5.2 Deployment Steps
1. **Compile ARM Binary**:
   ```bash
   make kindle
   ```
2. **Transfer Files via SCP**:
   ```bash
   scp bin/dino root@192.168.2.2:/mnt/us/dino/dino
   scp scripts/run_kindle.sh root@192.168.2.2:/mnt/us/dino/run.sh
   ```
3. **Run on Kindle via SSH**:
   ```bash
   ssh root@192.168.2.2
   cd /mnt/us/dino
   chmod +x dino run.sh
   ./run.sh
   ```

### 5.3 Launcher Script (`run.sh`) Behavior
The launcher script performs the following critical tasks:
1. Stops the Kindle Java framework (`/etc/init.d/framework stop`) to free ~120 MB RAM and silence OS screen updates.
2. Sets screen orientation to portrait (`echo 0 > /sys/class/graphics/fb0/rotate`).
3. Launches `/mnt/us/dino/dino`.
4. Upon game termination, restarts the Kindle framework (`/etc/init.d/framework start`).
