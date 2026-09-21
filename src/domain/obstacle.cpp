#include "domain/obstacle.h"

namespace dino::domain {

ScreenCoordinate Obstacle::position() const {
    return ScreenCoordinate(position_.x().to_pixels(), position_.y());
}

Dimensions Obstacle::dimensions() const {
    if (type_ == ObstacleType::SmallCactus) {
        return Dimensions(SMALL_CACTUS_WIDTH, SMALL_CACTUS_HEIGHT);
    }
    if (type_ == ObstacleType::LargeCactus) {
        return Dimensions(LARGE_CACTUS_WIDTH, LARGE_CACTUS_HEIGHT);
    }
    return Dimensions(PTERODACTYL_WIDTH, PTERODACTYL_HEIGHT);
}

BoundingBox Obstacle::visual_box() const {
    return BoundingBox(position(), dimensions());
}

BoundingBox Obstacle::hitbox() const {
    // Inset by 3px on all sides for player fairness.
    return visual_box().inset(3);
}

bool Obstacle::is_off_screen() const {
    const int32_t right_x = position().x() + dimensions().width();
    return right_x < 0;
}

void Obstacle::update(FixedPoint speed) {
    position_ = position_.move_left(speed);
}

}  // namespace dino::domain
