#ifndef DINO_GAME_INPUT_STATE_H_
#define DINO_GAME_INPUT_STATE_H_

#include "game/input_action.h"

namespace dino::game {

/**
 * @brief Momentary edge-triggered action requests.
 */
class MomentaryTriggers {
public:
    constexpr MomentaryTriggers()
        : jump_requested_(false), restart_requested_(false) {}

    [[nodiscard]] constexpr bool jump() const { return jump_requested_; }
    [[nodiscard]] constexpr bool restart() const { return restart_requested_; }

    void trigger_jump() { jump_requested_ = true; }
    void trigger_restart() { restart_requested_ = true; }
    void clear() {
        jump_requested_ = false;
        restart_requested_ = false;
    }

private:
    bool jump_requested_;
    bool restart_requested_;
};

/**
 * @brief Level-sensitive and application-level lifecycle triggers.
 */
class SessionTriggers {
public:
    constexpr SessionTriggers()
        : quit_requested_(false), ducking_active_(false) {}

    [[nodiscard]] constexpr bool quit() const { return quit_requested_; }
    [[nodiscard]] constexpr bool ducking() const { return ducking_active_; }

    void trigger_quit() { quit_requested_ = true; }
    void set_ducking(bool active) { ducking_active_ = active; }

private:
    bool quit_requested_;
    bool ducking_active_;
};

/**
 * @brief Manages edge-triggered jumps, held duck states, and quit triggers.
 */
class InputState {
public:
    InputState() = default;

    [[nodiscard]] bool is_jump_requested() const {
        return momentary_.jump();
    }
    [[nodiscard]] bool is_restart_requested() const {
        return momentary_.restart();
    }
    [[nodiscard]] bool is_quit_requested() const {
        return session_.quit();
    }
    [[nodiscard]] bool is_ducking() const {
        return session_.ducking();
    }

    void handle_jump_press();
    void handle_duck_press();
    void handle_duck_release();
    void handle_restart_press();
    void handle_quit_press();
    void consume_momentary_triggers();

private:
    MomentaryTriggers momentary_;
    SessionTriggers session_;
};

}  // namespace dino::game

#endif  // DINO_GAME_INPUT_STATE_H_
