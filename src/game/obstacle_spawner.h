#ifndef DINO_GAME_OBSTACLE_SPAWNER_H_
#define DINO_GAME_OBSTACLE_SPAWNER_H_

#include "domain/cloud_collection.h"
#include "domain/game_values.h"
#include "domain/obstacle_collection.h"
#include "domain/random_generator.h"

namespace dino::game {

/**
 * @brief Procedurally generates obstacles and clouds respecting fairness gaps.
 */
class ObstacleSpawner {
public:
    /**
     * @brief Constructs spawner with deterministic random seed.
     * @param seed Pseudorandom generator seed.
     */
    explicit ObstacleSpawner(uint32_t seed = 0x87654321U);

    /**
     * @brief Spawns obstacles and clouds as needed based on gap distance and score.
     * @param obstacles Active obstacle collection to populate.
     * @param clouds Active cloud collection to populate.
     * @param current_score Current score for difficulty progression.
     */
    void update(domain::ObstacleCollection& obstacles,
                domain::CloudCollection& clouds,
                domain::ScoreValue current_score);

    /**
     * @brief Resets spawner gap distance and random state.
     */
    void reset();

private:
    [[nodiscard]] domain::ObstacleType select_obstacle_type(domain::ScoreValue score);
    void maybe_spawn_obstacle(domain::ObstacleCollection& obstacles,
                              domain::ScoreValue score);
    void maybe_spawn_cloud(domain::CloudCollection& clouds);

    int32_t next_gap_pixels_;
    domain::RandomGenerator random_;
};

}  // namespace dino::game

#endif  // DINO_GAME_OBSTACLE_SPAWNER_H_
