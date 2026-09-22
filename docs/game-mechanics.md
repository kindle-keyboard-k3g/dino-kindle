# Game Mechanics & Physics

## 1. Deterministic 16.16 Fixed-Point Physics

The Kindle Keyboard's Freescale i.MX353 SoC uses a soft-float ABI (`-mfloat-abi=softfp`), where floating-point math incurs software emulation penalty. Dino Kindle uses **16.16 signed fixed-point arithmetic** (`domain::FixedPoint`) for all physical positions, velocities, and gravity calculations:

$$\text{Value} = \frac{\text{raw\_value}}{2^{16}} = \frac{\text{raw\_value}}{65536}$$

### 1.1 Precision & Conversions
- `ONE_RAW = 65536`: Unit value $1.0$.
- `to_int()`: Arithmetic right-shift by 16 bits (`raw_ >> 16`).
- Rounding: Truncation towards negative infinity or integer conversion.
- Addition / Subtraction: Native 32-bit integer arithmetic.
- Multiplication: `(int64_t(a.raw()) * b.raw()) >> 16` avoiding 32-bit overflow.

---

## 2. Dinosaur Kinematics (`PlayerPhysics`)

The dinosaur's vertical trajectory is modeled with discrete time integration executed at 80 ms intervals ($12.5\text{ FPS}$):

### 2.1 Physics Constants
| Constant | Value (Pixels / Frame) | FixedPoint Raw Value | Description |
| :--- | :--- | :--- | :--- |
| **Ground Baseline** | $Y = 473$ | $30998528$ | Standing foot position on ground ($520 - 47$) |
| **Jump Impulse** | $-12.0$ | $-786432$ | Initial upward velocity |
| **Gravity** | $+1.2$ | $+78643$ | Downward acceleration per tick |
| **Terminal Velocity** | $+16.0$ | $+1048576$ | Maximum falling speed |

### 2.2 Integration Step
Each tick where the player is airborne:
1. $\text{velocity}_y \leftarrow \min(\text{velocity}_y + \text{gravity}, \text{terminal\_velocity})$
2. $\text{pos}_y \leftarrow \text{pos}_y + \text{velocity}_y$
3. If $\text{pos}_y \ge \text{baseline}$:
   - $\text{pos}_y \leftarrow \text{baseline}$
   - $\text{velocity}_y \leftarrow 0$
   - State transitions from `Jumping` to `Running`

### 2.3 Ducking Mechanics
When Ducking is active (via 5-Way Down or Bottom Page Turn button):
- Dinosaur posture switches to 55 × 26 pixels.
- Head height drops by 21 pixels, allowing the player to pass under High Pterodactyls.
- If duck is triggered while airborne, gravity drops faster to allow rapid descent.

---

## 3. Speed Progression & Difficulty Scaling

Speed scales dynamically as distance increases to match the official Chrome Dino curve:

$$\text{Speed}(s) = \min\left(8 + \frac{s}{500}, 18\right) \text{ pixels per frame}$$

- **Starting Speed**: 8 pixels/frame ($100\text{ px/sec}$).
- **Maximum Speed**: 18 pixels/frame ($225\text{ px/sec}$).
- **Interval**: 80 ms per frame.

---

## 4. Obstacle Spawning & Procedural Fairness (`ObstacleSpawner`)

Obstacles are generated deterministically using a Linear Congruential Generator (`RandomGenerator`):
$$X_{n+1} = (1103515245 \cdot X_n + 12345) \pmod{2^{31}}$$

### 4.1 Obstacle Types & Dimensions
| Type | Visual Box ($W \times H$) | Inset Hitbox ($W \times H$) | Spawning Altitude |
| :--- | :--- | :--- | :--- |
| **Small Cactus (Single)** | 17 × 35 px | 13 × 31 px | Ground ($Y = 485$) |
| **Small Cactus (Double)** | 34 × 35 px | 30 × 31 px | Ground ($Y = 485$) |
| **Large Cactus (Single)** | 25 × 50 px | 21 × 46 px | Ground ($Y = 470$) |
| **Large Cactus (Group)** | 50 × 50 px | 46 × 46 px | Ground ($Y = 470$) |
| **Pterodactyl (Low)** | 46 × 40 px | 40 × 34 px | Low Air ($Y = 470$) — Must Jump |
| **Pterodactyl (High)** | 46 × 40 px | 40 × 34 px | High Air ($Y = 440$) — Must Duck or Stand |

### 4.2 Spawn Gap Fairness
To prevent impossible jumps, `ObstacleSpawner` enforces minimum and maximum inter-obstacle spacing:
$$\text{Min Gap} = \max(220, 20 \times \text{Speed}) \text{ pixels}$$
$$\text{Max Gap} = \text{Min Gap} + 230 \text{ pixels}$$

Pterodactyls only spawn once the score exceeds **300 points**.

---

## 5. Collision Detection (`CollisionDetector`)

To ensure player fairness and avoid "phantom edge" collisions, collision detection uses **inset hitboxes**:

1. **Player Inset**: The bounding box of the Dino is inset by:
   - Standing: 4px horizontal, 4px vertical margin.
   - Ducking: 4px horizontal, 2px vertical margin.
2. **Obstacle Inset**: Cacti and Pterodactyls are inset by 2px in all directions.
3. **AABB Intersection**:
```cpp
bool CollisionDetector::has_collision(const Player& player,
                                      const ObstacleCollection& obstacles) const {
    const BoundingBox player_box = player.hitbox();
    for (int32_t i = 0; i < obstacles.count(); ++i) {
        if (player_box.intersects(obstacles.get(i).hitbox())) {
            return true;
        }
    }
    return false;
}
```

---

## 6. Score Milestones & Day/Night Inversion

1. **Score Accumulation**: Points increment based on distance traveled ($1 \text{ point} \approx 40 \text{ distance units}$).
2. **700-Point Milestone**:
   - When `score % 700 == 0` (and `score > 0`), a milestone is triggered.
   - The theme inverts: `Theme::invert()` toggles foreground and background palettes (`White` $\leftrightarrow$ `Black`).
   - `RefreshCoordinator` initiates a 500 ms simulation freeze and issues a full GC16 hardware refresh to clear ghosting.
