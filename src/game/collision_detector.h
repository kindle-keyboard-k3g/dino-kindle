#ifndef DINO_GAME_COLLISION_DETECTOR_H_
#define DINO_GAME_COLLISION_DETECTOR_H_

#include "domain/obstacle_collection.h"
#include "domain/player.h"

namespace dino::game {

/**
 * @brief Evaluates spatial intersection between player and obstacles.
 */
class CollisionDetector {
public:
    [[nodiscard]] static bool has_collision(
        const domain::Player& player,
        const domain::ObstacleCollection& obstacles);
};

}  // namespace dino::game

#endif  // DINO_GAME_COLLISION_DETECTOR_H_
