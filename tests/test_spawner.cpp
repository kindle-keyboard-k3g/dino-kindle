#include "test_framework.h"
#include "game/obstacle_spawner.h"

using namespace dino;

DINO_TEST_CASE(TestSpawnerEnforcesMinimumGap) {
    game::ObstacleSpawner spawner(0x1234U);
    domain::ObstacleCollection obstacles;
    domain::CloudCollection clouds;

    spawner.update(obstacles, clouds, domain::ScoreValue(100));
    ASSERT_EQ(obstacles.count(), 1);

    // Immediate second update should NOT spawn because obstacle is still at right screen edge
    spawner.update(obstacles, clouds, domain::ScoreValue(100));
    ASSERT_EQ(obstacles.count(), 1);

    // Advance obstacle past max gap threshold
    obstacles.update_all(domain::FixedPoint::from_pixels(500));
    spawner.update(obstacles, clouds, domain::ScoreValue(100));
    ASSERT_EQ(obstacles.count(), 2);
}

DINO_TEST_CASE(TestSpawnerPterodactylThreshold) {
    game::ObstacleSpawner spawner(0x9999U);
    domain::ObstacleCollection obstacles;
    domain::CloudCollection clouds;

    // Below 300 points, only cacti should appear
    for (int i = 0; i < 20; ++i) {
        obstacles.clear();
        spawner.update(obstacles, clouds, domain::ScoreValue(150));
        ASSERT_TRUE(obstacles.at(0).type() == domain::ObstacleType::SmallCactus ||
                    obstacles.at(0).type() == domain::ObstacleType::LargeCactus);
    }
}
