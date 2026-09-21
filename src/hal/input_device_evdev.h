#ifndef DINO_HAL_INPUT_DEVICE_EVDEV_H_
#define DINO_HAL_INPUT_DEVICE_EVDEV_H_

#include "hal/input_device.h"

#include <vector>

namespace dino::hal {

/**
 * @brief Kindle Keyboard evdev multiplexer polling event0, event1, and event2.
 */
class InputDeviceEvdev : public InputDevice {
public:
    InputDeviceEvdev();
    ~InputDeviceEvdev() override;

    InputDeviceEvdev(const InputDeviceEvdev&) = delete;
    InputDeviceEvdev& operator=(const InputDeviceEvdev&) = delete;

    void poll_events(game::InputState& state) override;

private:
    void open_devices();
    void close_devices();
    void process_event(uint16_t code, int32_t value, game::InputState& state);

    std::vector<int> descriptors_;
    bool grab_active_;
};

}  // namespace dino::hal

#endif  // DINO_HAL_INPUT_DEVICE_EVDEV_H_
