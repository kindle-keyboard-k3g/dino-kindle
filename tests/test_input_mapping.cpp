#include "test_framework.h"
#include "game/input_state.h"

using namespace dino;

DINO_TEST_CASE(TestInputEdgeTriggeredJump) {
    game::InputState input;
    ASSERT_FALSE(input.is_jump_requested());

    input.handle_jump_press();
    ASSERT_TRUE(input.is_jump_requested());

    input.consume_momentary_triggers();
    ASSERT_FALSE(input.is_jump_requested());
}

DINO_TEST_CASE(TestInputLevelTriggeredDuck) {
    game::InputState input;
    ASSERT_FALSE(input.is_ducking());

    input.handle_duck_press();
    ASSERT_TRUE(input.is_ducking());

    // Holding duck remains active across consuming momentary triggers
    input.consume_momentary_triggers();
    ASSERT_TRUE(input.is_ducking());

    input.handle_duck_release();
    ASSERT_FALSE(input.is_ducking());
}

DINO_TEST_CASE(TestInputQuitAndRestartTriggers) {
    game::InputState input;
    ASSERT_FALSE(input.is_quit_requested());
    ASSERT_FALSE(input.is_restart_requested());

    input.handle_quit_press();
    ASSERT_TRUE(input.is_quit_requested());

    input.handle_restart_press();
    ASSERT_TRUE(input.is_restart_requested());
}
