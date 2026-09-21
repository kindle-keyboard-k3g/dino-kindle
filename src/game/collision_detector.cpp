#include "game/collision_detector.h"

namespace dino::game {

bool CollisionDetector::has_collision(
    const domain::Player& player,
    const domain::ObstacleCollection& obstacles) {
    const domain::BoundingBox player_box = player.hitbox();
    const size_t count = obstacles.count();
    for (size_t i = 0; i < count; ++i) {
        const domain::BoundingBox obstacle_box = obstacles.at(i).hitbox();
        if (player_box.intersects(obstacle_box)) {
            return true;
        }
    }
    return false;
}

}  // namespace dino::game
