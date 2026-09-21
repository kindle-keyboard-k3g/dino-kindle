# Plan: Replace ASCII/Placeholder UI with Real Chrome Dino Game Assets

## 1. Context & Need
The current implementation of `dino-kindle` uses rudimentary approximations for in-game visual elements:
- "GAME OVER" and "PRESS SPACE OR UP TO RESTART" are drawn using a basic 5×7 monospace ASCII font (`BitmapFont`) instead of the iconic pixel-art "GAME OVER" graphic banner and circular Restart button from the official Chromium game.
- The ground is currently rendered as a plain 2px line with hardcoded arbitrary dots (`draw_ground_bumps`) instead of the rich, textured terrain patterns from the authentic Chrome Dino spritesheet.
- The score numbers use the standard 5×7 ASCII font instead of the authentic tall pixel numbers (character glyphs tailored to the Chromium retro runner).
- Dino and obstacle sprites, while functional, can be upgraded to exact pixel-accurate 1-bit bitmasks corresponding to the official Chromium 1x assets.

The goal of this task is to integrate **real, authentic Chromium Dino game assets** across the entire graphics pipeline while strictly respecting:
1. The Kindle Keyboard hardware constraints (Freescale i.MX353, 600×800 16-level grayscale E-Ink display, packed 4bpp nibbles, 12.5 FPS / 80ms step).
2. Clean Code and Object Calisthenics (all files < 200 lines, functions ≤ 15 lines, classes ≤ 100 lines, ≤ 2 fields per class, zero `else` keywords).
3. Zero dynamic memory allocation during gameplay.

---

## 2. Proposed Asset Upgrades

### 2.1 Authentic "GAME OVER" Graphic Sprite
- In Chromium `offline-sprite-definitions.js`, `TEXT_SPRITE` ("GAME OVER") is an authentic pixel-art graphic: **191 × 11 pixels** (or scaled 192 × 11).
- We will encode the exact 1-bit packed bitmap mask for the authentic "GAME OVER" banner.
- Replaces the generic ASCII text string `"G A M E   O V E R"`.

### 2.2 Authentic Circular Restart Button Sprite
- In Chromium, restarting is represented by the circular arrow icon button: **36 × 32 pixels**.
- We will encode the exact 1-bit packed bitmap mask for the Restart button.
- Rendered centered below the "GAME OVER" banner.

### 2.3 Authentic Digital Score Number Glyphs
- The iconic Chromium score numbers: **10 × 13 pixels** (digits `0`–`9`, plus `H` and `I` for `HI`).
- Replaces the tiny 5×7 ASCII font in the HUD, providing crisp, authentic readability on the 6.0" E-Ink Pearl screen.

### 2.4 Authentic Procedural Ground Terrain Slices
- Authentic Chromium terrain contains distinct ground bumps, rocks, and star/cross speckles.
- We will encode authentic ground segment bitmasks (slices of 128 or 240 pixels) or tileable authentic ground chunks that scroll seamlessly at baseline $Y=520$.

### 2.5 Pixel-Perfect Dinosaur, Obstacle, and Cloud Sprites
- Verify and refine the 1-bit bitmasks in `SpriteData` for:
  - Dino Standing / Jumping (44 × 47)
  - Dino Running (Left / Right leg frames, 44 × 47)
  - Dino Ducking (Left / Right leg frames, 55 × 26)
  - Dino Crashed (X eye frame, 44 × 47)
  - Small Cactus (Single 17 × 35, Group 34 × 35)
  - Large Cactus (Single 25 × 50, Group 50 × 50)
  - Pterodactyl (Wing Up / Wing Down, 46 × 40)
  - Cloud (46 × 14)

---

## 3. Implementation Steps

### Step 1: Update `SpriteData` (`src/graphics/sprite_data.h` and `src/graphics/sprite_data.cpp`)
- Add authentic bitmasks:
  - `game_over_banner()`: $191 \times 11$ px bitmap mask.
  - `restart_button()`: $36 \times 32$ px bitmap mask.
  - `score_digit(int digit)`: $10 \times 13$ px bitmap mask for digits 0–9.
  - `score_hi()`: $20 \times 13$ px bitmap mask for "HI".
  - Refined ground texture tiles / slices.
- Keep file size strictly under 200 lines (split into `ui_sprites.h/.cpp` if needed to respect Object Calisthenics file length limits).

### Step 2: Create `ScoreRenderer` / Update `SceneRenderer`
- Render the HUD score (`HI XXXXX  YYYYY`) using the authentic $10 \times 13$ pixel digits instead of `BitmapFont`.
- In `render_game_over`:
  - Draw the authentic `game_over_banner` centered horizontally at $Y = 240$.
  - Draw the authentic `restart_button` centered horizontally below at $Y = 270$.
  - Provide a subtle key hint (`[SPACE / 5-WAY UP]`) using clean bitmap font at $Y = 320$.

### Step 3: Update `SpriteRenderer::draw_ground`
- Render authentic ground surface using repeating textured terrain slices instead of bare 3-pixel dots.

### Step 4: Verify Clean Code & Object Calisthenics
- Run `./scripts/check_limits.sh` to guarantee:
  - Zero `else` keywords in domain and game logic.
  - All files strictly $< 200$ lines.
  - Classes $\le 100$ lines with $\le 2$ instance variables.

### Step 5: Test Verification & Emulation
- Run unit tests: `make test` (ensure all tests pass, update sprite dimension assertions if any).
- Run AddressSanitizer: `make asan`.
- Run ANSI emulation: `make run-ansi`.
- Generate PPM frame export: `make host && ./bin/dino --ppm --frames 10` to visually inspect the pixel-perfect rendering.

---

## 4. Verification Plan
1. **Automated Unit Tests**: `make test` passes 100%.
2. **Memory Safety**: `make asan` reports zero leaks and zero undefined behavior.
3. **Limit Audit**: `./scripts/check_limits.sh` reports 0 violations.
4. **Visual Inspection**: PPM export renders the authentic "GAME OVER" banner, restart icon, digital score, and textured ground.
