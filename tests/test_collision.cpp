#include "test_framework.h"
#include "game/collision_detector.h"

using namespace dino;

DINO_TEST_CASE(TestPlayerCollidesWithCactusOnGround) {
    domain::Player player;
    domain::ObstacleCollection obstacles;
    // Place cactus directly at player's X position (X=50)
    obstacles.add(domain::Obstacle(
        domain::ObstacleType::SmallCactus,
        domain::FixedPoint::from_pixels(domain::Player::X_POSITION)));

    ASSERT_TRUE(game::CollisionDetector::has_collision(player, obstacles));
}

DINO_TEST_CASE(TestJumpingPlayerClearsCactus) {
    domain::Player player;
    player.jump();
    // Advance physics to peak height (~80px above ground)
    for (int i = 0; i < 7; ++i) {
        player.update();
    }
    domain::ObstacleCollection obstacles;
    obstacles.add(domain::Obstacle(
        domain::ObstacleType::SmallCactus,
        domain::FixedPoint::from_pixels(domain::Player::X_POSITION)));

    ASSERT_FALSE(game::CollisionDetector::has_collision(player, obstacles));
}

DINO_TEST_CASE(TestDuckingPassesUnderHighPterodactyl) {
    domain::Player player;
    domain::ObstacleCollection obstacles;
    obstacles.add(domain::Obstacle(
        domain::ObstacleType::PterodactylHigh,
        domain::FixedPoint::from_pixels(domain::Player::X_POSITION)));

    // Standing player collides with high pterodactyl
    ASSERT_TRUE(game::CollisionDetector::has_collision(player, obstacles));

    // Ducking player passes underneath cleanly!
    player.duck();
    ASSERT_FALSE(game::CollisionDetector::has_collision(player, obstacles));
}
