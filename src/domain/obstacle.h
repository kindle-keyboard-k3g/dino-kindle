#ifndef DINO_DOMAIN_OBSTACLE_H_
#define DINO_DOMAIN_OBSTACLE_H_

#include "domain/game_values.h"
#include "domain/geometry.h"

namespace dino::domain {

enum class ObstacleType : uint8_t {
    SmallCactus,
    LargeCactus,
    PterodactylLow,
    PterodactylHigh
};

/**
 * @brief Spatial position of an obstacle on the screen.
 */
class ObstaclePosition {
public:
    constexpr ObstaclePosition()
        : x_(FixedPoint::from_pixels(0)), y_(0) {}
    constexpr ObstaclePosition(FixedPoint x, int32_t y)
        : x_(x), y_(y) {}

    [[nodiscard]] constexpr FixedPoint x() const { return x_; }
    [[nodiscard]] constexpr int32_t y() const { return y_; }

    [[nodiscard]] constexpr ObstaclePosition move_left(FixedPoint speed) const {
        return ObstaclePosition(x_ - speed, y_);
    }

private:
    FixedPoint x_;
    int32_t y_;
};

/**
 * @brief Obstacle representation (cactus or pterodactyl).
 */
class Obstacle {
public:
    static constexpr int32_t SMALL_CACTUS_WIDTH = 20;
    static constexpr int32_t SMALL_CACTUS_HEIGHT = 36;
    static constexpr int32_t LARGE_CACTUS_WIDTH = 32;
    static constexpr int32_t LARGE_CACTUS_HEIGHT = 48;
    static constexpr int32_t PTERODACTYL_WIDTH = 42;
    static constexpr int32_t PTERODACTYL_HEIGHT = 32;

    static constexpr int32_t GROUND_Y = 520;
    static constexpr int32_t PTERO_HIGH_Y = 455;
    static constexpr int32_t PTERO_LOW_Y = 485;

    constexpr Obstacle()
        : type_(ObstacleType::SmallCactus), position_{} {}
    constexpr Obstacle(ObstacleType type, FixedPoint start_x)
        : type_(type), position_(start_x, compute_y(type)) {}

    [[nodiscard]] constexpr ObstacleType type() const { return type_; }
    [[nodiscard]] ScreenCoordinate position() const;
    [[nodiscard]] Dimensions dimensions() const;
    [[nodiscard]] BoundingBox visual_box() const;
    [[nodiscard]] BoundingBox hitbox() const;
    [[nodiscard]] bool is_off_screen() const;

    void update(FixedPoint speed);

private:
    static constexpr int32_t compute_y(ObstacleType t) {
        if (t == ObstacleType::SmallCactus) {
            return GROUND_Y - SMALL_CACTUS_HEIGHT;
        }
        if (t == ObstacleType::LargeCactus) {
            return GROUND_Y - LARGE_CACTUS_HEIGHT;
        }
        if (t == ObstacleType::PterodactylHigh) {
            return PTERO_HIGH_Y;
        }
        return PTERO_LOW_Y;
    }

    ObstacleType type_;
    ObstaclePosition position_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_OBSTACLE_H_
