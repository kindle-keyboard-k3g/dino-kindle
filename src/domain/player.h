#ifndef DINO_DOMAIN_PLAYER_H_
#define DINO_DOMAIN_PLAYER_H_

#include "domain/game_values.h"
#include "domain/geometry.h"

namespace dino::domain {

enum class PlayerState : uint8_t {
    Running,
    Jumping,
    Ducking,
    Crashed
};

/**
 * @brief Encapsulates vertical motion, gravity, and ground collision.
 */
class PlayerPhysics {
public:
    static constexpr int32_t GROUND_Y = 520;
    static constexpr int32_t JUMP_IMPULSE_PIXELS = -22;
    static constexpr int32_t GRAVITY_PIXELS = 3;

    constexpr PlayerPhysics()
        : y_(FixedPoint::from_pixels(GROUND_Y)),
          velocity_y_(FixedPoint::from_pixels(0)) {}

    /// Vertical position in fixed point.
    [[nodiscard]] constexpr FixedPoint y() const { return y_; }
    /// Vertical velocity in fixed point.
    [[nodiscard]] constexpr FixedPoint velocity_y() const { return velocity_y_; }
    /// Checks if player feet touch or rest upon ground line.
    [[nodiscard]] constexpr bool is_on_ground() const {
        return y_ >= FixedPoint::from_pixels(GROUND_Y) &&
               velocity_y_ >= FixedPoint::from_pixels(0);
    }

    /// Applies jump vertical impulse if grounded.
    void apply_jump();
    /// Advances gravity integration and ground collision clamping.
    void update();
    /// Resets vertical position to ground and clears velocity.
    void reset();

private:
    FixedPoint y_;
    FixedPoint velocity_y_;
};

/**
 * @brief Encapsulates pose state and walking animation step.
 */
class PlayerAnimation {
public:
    constexpr PlayerAnimation()
        : state_(PlayerState::Running), leg_step_(0) {}

    /// Current animation pose state.
    [[nodiscard]] constexpr PlayerState state() const { return state_; }
    /// Walking leg phase step (0 or 1).
    [[nodiscard]] constexpr uint8_t leg_step() const { return leg_step_; }

    /// Sets current pose state.
    void set_state(PlayerState new_state) { state_ = new_state; }
    /// Toggles leg step between 0 and 1.
    void advance_step() { leg_step_ = static_cast<uint8_t>(1 - leg_step_); }
    /// Resets pose to Running and leg step to 0.
    void reset() {
        state_ = PlayerState::Running;
        leg_step_ = 0;
    }

private:
    PlayerState state_;
    uint8_t leg_step_;
};

/**
 * @brief Complete Dino player aggregate.
 */
class Player {
public:
    static constexpr int32_t X_POSITION = 50;
    static constexpr int32_t STANDING_WIDTH = 44;
    static constexpr int32_t STANDING_HEIGHT = 48;
    static constexpr int32_t DUCKING_WIDTH = 56;
    static constexpr int32_t DUCKING_HEIGHT = 28;
    static constexpr int32_t HITBOX_INSET = 3;

    Player() = default;

    /// Upper-left screen coordinate of Dino sprite.
    [[nodiscard]] ScreenCoordinate position() const;
    /// Current bounding dimensions based on standing or ducking.
    [[nodiscard]] Dimensions dimensions() const;
    /// Visual box on screen for rendering.
    [[nodiscard]] BoundingBox visual_box() const;
    /// Inset collision box for gameplay collision testing.
    [[nodiscard]] BoundingBox hitbox() const;
    /// Current pose state (Running, Jumping, Ducking, Crashed).
    [[nodiscard]] PlayerState state() const { return animation_.state(); }
    /// Walking animation frame step.
    [[nodiscard]] uint8_t leg_step() const { return animation_.leg_step(); }
    /// Indicates if Dino is on the ground.
    [[nodiscard]] bool is_on_ground() const { return physics_.is_on_ground(); }

    /// Initiates jump if currently on the ground.
    void jump();
    /// Switches pose to ducking.
    void duck();
    /// Restores standing or running pose if ducking.
    void stand();
    /// Switches pose to crashed.
    void crash();
    /// Advances physics integration and leg animation.
    void update();
    /// Resets player to ground running state.
    void reset();

private:
    PlayerPhysics physics_;
    PlayerAnimation animation_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_PLAYER_H_
