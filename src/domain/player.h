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

    [[nodiscard]] constexpr FixedPoint y() const { return y_; }
    [[nodiscard]] constexpr FixedPoint velocity_y() const { return velocity_y_; }
    [[nodiscard]] constexpr bool is_on_ground() const {
        return y_ >= FixedPoint::from_pixels(GROUND_Y) &&
               velocity_y_ >= FixedPoint::from_pixels(0);
    }

    void apply_jump();
    void update();
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

    [[nodiscard]] constexpr PlayerState state() const { return state_; }
    [[nodiscard]] constexpr uint8_t leg_step() const { return leg_step_; }

    void set_state(PlayerState new_state) { state_ = new_state; }
    void advance_step() { leg_step_ = static_cast<uint8_t>(1 - leg_step_); }
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

    [[nodiscard]] ScreenCoordinate position() const;
    [[nodiscard]] Dimensions dimensions() const;
    [[nodiscard]] BoundingBox visual_box() const;
    [[nodiscard]] BoundingBox hitbox() const;
    [[nodiscard]] PlayerState state() const { return animation_.state(); }
    [[nodiscard]] uint8_t leg_step() const { return animation_.leg_step(); }
    [[nodiscard]] bool is_on_ground() const { return physics_.is_on_ground(); }

    void jump();
    void duck();
    void stand();
    void crash();
    void update();
    void reset();

private:
    PlayerPhysics physics_;
    PlayerAnimation animation_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_PLAYER_H_
