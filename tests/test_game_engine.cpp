#include "test_framework.h"
#include "game/game_engine.h"

using namespace dino;

DINO_TEST_CASE(TestGameEngineNormalProgression) {
    game::GameEngine engine(0x1234U);
    game::InputState input;

    ASSERT_FALSE(engine.is_game_over());
    const int32_t initial_score = engine.snapshot().status().current_score().points();

    // Step 20 frames
    for (int i = 0; i < 20; ++i) {
        engine.step(input);
    }

    const int32_t current_score = engine.snapshot().status().current_score().points();
    ASSERT_TRUE(current_score >= initial_score);
}

DINO_TEST_CASE(TestGameEngineCollisionAndRestart) {
    game::GameEngine engine(0x1234U);
    game::InputState input;

    // Manually place obstacle directly at player
    engine.inject_obstacle_for_testing(domain::Obstacle(
        domain::ObstacleType::SmallCactus,
        domain::FixedPoint::from_pixels(domain::Player::X_POSITION)));

    engine.step(input);
    ASSERT_TRUE(engine.is_game_over());

    // Steps during game over do not advance score
    const int32_t score_at_death = engine.snapshot().status().current_score().points();
    engine.step(input);
    ASSERT_EQ(engine.snapshot().status().current_score().points(), score_at_death);

    // Trigger restart
    input.handle_restart_press();
    engine.step(input);
    ASSERT_FALSE(engine.is_game_over());
}
