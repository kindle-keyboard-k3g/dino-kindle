#include "test_framework.h"
#include "domain/obstacle_collection.h"

using namespace dino::domain;

DINO_TEST_CASE(TestObstacleDimensionsAndPositions) {
    Obstacle small_c(ObstacleType::SmallCactus, FixedPoint::from_pixels(300));
    ASSERT_EQ(small_c.dimensions().width(), Obstacle::SMALL_CACTUS_WIDTH);
    ASSERT_EQ(small_c.dimensions().height(), Obstacle::SMALL_CACTUS_HEIGHT);

    Obstacle large_c(ObstacleType::LargeCactus, FixedPoint::from_pixels(300));
    ASSERT_EQ(large_c.dimensions().width(), Obstacle::LARGE_CACTUS_WIDTH);
    ASSERT_EQ(large_c.dimensions().height(), Obstacle::LARGE_CACTUS_HEIGHT);

    Obstacle ptero_high(ObstacleType::PterodactylHigh, FixedPoint::from_pixels(300));
    ASSERT_EQ(ptero_high.position().y(), Obstacle::PTERO_HIGH_Y);

    Obstacle ptero_low(ObstacleType::PterodactylLow, FixedPoint::from_pixels(300));
    ASSERT_EQ(ptero_low.position().y(), Obstacle::PTERO_LOW_Y);
}

DINO_TEST_CASE(TestObstacleMovementAndPruning) {
    ObstacleCollection collection;
    collection.add(Obstacle(ObstacleType::SmallCactus, FixedPoint::from_pixels(50)));
    ASSERT_EQ(collection.count(), 1);

    collection.update_all(FixedPoint::from_pixels(30));
    ASSERT_EQ(collection.at(0).position().x(), 20);

    // Move past left edge of screen
    collection.update_all(FixedPoint::from_pixels(100));
    ASSERT_TRUE(collection.at(0).is_off_screen());

    collection.prune_off_screen();
    ASSERT_EQ(collection.count(), 0);
    ASSERT_TRUE(collection.is_empty());
}
