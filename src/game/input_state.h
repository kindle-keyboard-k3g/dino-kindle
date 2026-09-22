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

    /// Checks if jump was requested.
    [[nodiscard]] constexpr bool jump() const { return jump_requested_; }
    /// Checks if restart was requested.
    [[nodiscard]] constexpr bool restart() const { return restart_requested_; }

    /// Sets jump requested flag.
    void trigger_jump() { jump_requested_ = true; }
    /// Sets restart requested flag.
    void trigger_restart() { restart_requested_ = true; }
    /// Clears both momentary flags.
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

    /// Checks if application quit was requested.
    [[nodiscard]] constexpr bool quit() const { return quit_requested_; }
    /// Checks if duck button is actively held down.
    [[nodiscard]] constexpr bool ducking() const { return ducking_active_; }

    /// Sets application quit flag.
    void trigger_quit() { quit_requested_ = true; }
    /// Updates ducking held status.
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

    /// Checks if a jump was requested this frame.
    [[nodiscard]] bool is_jump_requested() const {
        return momentary_.jump();
    }
    /// Checks if restart was requested this frame.
    [[nodiscard]] bool is_restart_requested() const {
        return momentary_.restart();
    }
    /// Checks if quit was requested this frame.
    [[nodiscard]] bool is_quit_requested() const {
        return session_.quit();
    }
    /// Checks if duck button is currently held down.
    [[nodiscard]] bool is_ducking() const {
        return session_.ducking();
    }

    /// Handles jump key press event.
    void handle_jump_press();
    /// Handles duck key press event.
    void handle_duck_press();
    /// Handles duck key release event.
    void handle_duck_release();
    /// Handles restart key press event.
    void handle_restart_press();
    /// Handles quit key press event.
    void handle_quit_press();
    /// Consumes momentary edge triggers after processing.
    void consume_momentary_triggers();

private:
    MomentaryTriggers momentary_;
    SessionTriggers session_;
};

}  // namespace dino::game

#endif  // DINO_GAME_INPUT_STATE_H_
