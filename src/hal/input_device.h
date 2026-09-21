#ifndef DINO_HAL_INPUT_DEVICE_H_
#define DINO_HAL_INPUT_DEVICE_H_

#include "game/input_state.h"

namespace dino::hal {

/**
 * @brief Abstract peripheral interface polling hardware input events.
 */
class InputDevice {
public:
    virtual ~InputDevice() = default;

    /**
     * @brief Polls pending hardware events and updates input state.
     */
    virtual void poll_events(game::InputState& state) = 0;
};

}  // namespace dino::hal

#endif  // DINO_HAL_INPUT_DEVICE_H_
