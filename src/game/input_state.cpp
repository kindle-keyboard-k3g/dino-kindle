#include "game/input_state.h"

namespace dino::game {

void InputState::handle_jump_press() {
    momentary_.trigger_jump();
}

void InputState::handle_duck_press() {
    session_.set_ducking(true);
}

void InputState::handle_duck_release() {
    session_.set_ducking(false);
}

void InputState::handle_restart_press() {
    momentary_.trigger_restart();
}

void InputState::handle_quit_press() {
    session_.trigger_quit();
}

void InputState::consume_momentary_triggers() {
    momentary_.clear();
}

}  // namespace dino::game
