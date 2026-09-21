#include "game/obstacle_spawner.h"
#include "game/game_rules.h"

namespace dino::game {

ObstacleSpawner::ObstacleSpawner(uint32_t seed)
    : next_gap_pixels_(GameRules::MIN_SPAWN_GAP_PIXELS),
      random_(seed) {}

domain::ObstacleType ObstacleSpawner::select_obstacle_type(domain::ScoreValue score) {
    if (score.points() < GameRules::PTERO_MIN_SCORE) {
        const int32_t roll = random_.next_range(0, 1);
        if (roll == 0) {
            return domain::ObstacleType::SmallCactus;
        }
        return domain::ObstacleType::LargeCactus;
    }
    const int32_t roll = random_.next_range(0, 3);
    if (roll == 0) {
        return domain::ObstacleType::SmallCactus;
    }
    if (roll == 1) {
        return domain::ObstacleType::LargeCactus;
    }
    if (roll == 2) {
        return domain::ObstacleType::PterodactylLow;
    }
    return domain::ObstacleType::PterodactylHigh;
}

void ObstacleSpawner::maybe_spawn_obstacle(
    domain::ObstacleCollection& obstacles,
    domain::ScoreValue score) {
    if (obstacles.is_full()) {
        return;
    }
    if (!obstacles.is_empty()) {
        const size_t last_index = obstacles.count() - 1;
        const int32_t last_x = obstacles.at(last_index).position().x();
        if (last_x > GameRules::SCREEN_WIDTH - next_gap_pixels_) {
            return;
        }
    }
    const domain::ObstacleType type = select_obstacle_type(score);
    const domain::FixedPoint spawn_x =
        domain::FixedPoint::from_pixels(GameRules::SCREEN_WIDTH);
    obstacles.add(domain::Obstacle(type, spawn_x));
    next_gap_pixels_ = random_.next_range(
        GameRules::MIN_SPAWN_GAP_PIXELS, GameRules::MAX_SPAWN_GAP_PIXELS);
}

void ObstacleSpawner::maybe_spawn_cloud(domain::CloudCollection& clouds) {
    if (clouds.is_full()) {
        return;
    }
    const int32_t roll = random_.next_range(0, 40);
    if (roll != 0) {
        return;
    }
    const int32_t cloud_y = random_.next_range(80, 260);
    const domain::FixedPoint spawn_x =
        domain::FixedPoint::from_pixels(GameRules::SCREEN_WIDTH);
    clouds.add(domain::Cloud(spawn_x, cloud_y));
}

void ObstacleSpawner::update(domain::ObstacleCollection& obstacles,
                             domain::CloudCollection& clouds,
                             domain::ScoreValue current_score) {
    maybe_spawn_obstacle(obstacles, current_score);
    maybe_spawn_cloud(clouds);
}

void ObstacleSpawner::reset() {
    next_gap_pixels_ = GameRules::MIN_SPAWN_GAP_PIXELS;
}

}  // namespace dino::game
