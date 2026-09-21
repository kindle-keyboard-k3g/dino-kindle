#ifndef DINO_GAME_INPUT_ACTION_H_
#define DINO_GAME_INPUT_ACTION_H_

#include <cstdint>

namespace dino::game {

/**
 * @brief Discrete player control intents.
 */
enum class InputAction : uint8_t {
    None,
    Jump,
    DuckStart,
    DuckStop,
    Restart,
    Quit
};

}  // namespace dino::game

#endif  // DINO_GAME_INPUT_ACTION_H_
