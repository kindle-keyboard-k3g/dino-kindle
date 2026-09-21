#include "test_framework.h"
#include "domain/player.h"

using namespace dino::domain;

DINO_TEST_CASE(TestPlayerInitialState) {
    Player player;
    ASSERT_TRUE(player.is_on_ground());
    ASSERT_EQ(player.state(), PlayerState::Running);
    ASSERT_EQ(player.dimensions().width(), Player::STANDING_WIDTH);
    ASSERT_EQ(player.dimensions().height(), Player::STANDING_HEIGHT);
    ASSERT_EQ(player.position().x(), Player::X_POSITION);
}

DINO_TEST_CASE(TestPlayerJumpTrajectory) {
    Player player;
    player.jump();
    ASSERT_EQ(player.state(), PlayerState::Jumping);
    ASSERT_FALSE(player.is_on_ground());

    const int32_t initial_y = player.position().y();
    player.update();
    ASSERT_TRUE(player.position().y() < initial_y); // Moved upward

    // Simulate jump until landing
    for (int i = 0; i < 20; ++i) {
        player.update();
    }
    ASSERT_TRUE(player.is_on_ground());
    ASSERT_EQ(player.state(), PlayerState::Running);
}

DINO_TEST_CASE(TestPlayerDucking) {
    Player player;
    player.duck();
    ASSERT_EQ(player.state(), PlayerState::Ducking);
    ASSERT_EQ(player.dimensions().width(), Player::DUCKING_WIDTH);
    ASSERT_EQ(player.dimensions().height(), Player::DUCKING_HEIGHT);

    player.stand();
    ASSERT_EQ(player.state(), PlayerState::Running);
    ASSERT_EQ(player.dimensions().height(), Player::STANDING_HEIGHT);
}

DINO_TEST_CASE(TestPlayerCrash) {
    Player player;
    player.crash();
    ASSERT_EQ(player.state(), PlayerState::Crashed);

    // After crashing, jump and duck are ignored
    player.jump();
    ASSERT_EQ(player.state(), PlayerState::Crashed);
    player.duck();
    ASSERT_EQ(player.state(), PlayerState::Crashed);
}
