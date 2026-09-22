#ifndef DINO_APPLICATION_HARDWARE_CONTEXT_H_
#define DINO_APPLICATION_HARDWARE_CONTEXT_H_

#include "application/application_options.h"
#include "hal/eink_controller.h"
#include "hal/frame_buffer.h"
#include "hal/input_device.h"
#include "persistence/high_score_store.h"

#include <memory>

namespace dino::application {

/**
 * @brief Bundles display hardware interfaces (framebuffer and EPDC).
 */
class DisplayBundle {
public:
    DisplayBundle(std::unique_ptr<hal::FrameBuffer> fb,
                  std::unique_ptr<hal::EinkController> epdc)
        : framebuffer_(std::move(fb)), eink_controller_(std::move(epdc)) {}

    /// Reference to active FrameBuffer device.
    [[nodiscard]] hal::FrameBuffer& framebuffer() { return *framebuffer_; }
    /// Reference to active EinkController device.
    [[nodiscard]] hal::EinkController& eink() { return *eink_controller_; }

private:
    std::unique_ptr<hal::FrameBuffer> framebuffer_;
    std::unique_ptr<hal::EinkController> eink_controller_;
};

/**
 * @brief Bundles peripheral and persistent storage interfaces.
 */
class PeripheralBundle {
public:
    PeripheralBundle(std::unique_ptr<hal::InputDevice> input,
                     std::unique_ptr<persistence::HighScoreStore> store)
        : input_device_(std::move(input)), high_score_store_(std::move(store)) {}

    /// Reference to active InputDevice.
    [[nodiscard]] hal::InputDevice& input() { return *input_device_; }
    /// Reference to active HighScoreStore.
    [[nodiscard]] persistence::HighScoreStore& store() { return *high_score_store_; }

private:
    std::unique_ptr<hal::InputDevice> input_device_;
    std::unique_ptr<persistence::HighScoreStore> high_score_store_;
};

/**
 * @brief Container and factory for hardware and fallback devices.
 */
class HardwareContext {
public:
    HardwareContext(DisplayBundle display, PeripheralBundle peripherals)
        : display_(std::move(display)), peripherals_(std::move(peripherals)) {}

    /**
     * @brief Creates appropriate HardwareContext based on CLI execution options.
     * @param options Parsed ApplicationOptions.
     * @return Unique pointer to configured HardwareContext.
     */
    [[nodiscard]] static std::unique_ptr<HardwareContext> create(
        const ApplicationOptions& options);

    /// Accesses display bundle (framebuffer and EPDC).
    [[nodiscard]] DisplayBundle& display() { return display_; }
    /// Accesses peripheral bundle (input device and high score store).
    [[nodiscard]] PeripheralBundle& peripherals() { return peripherals_; }

private:
    DisplayBundle display_;
    PeripheralBundle peripherals_;
};

}  // namespace dino::application

#endif  // DINO_APPLICATION_HARDWARE_CONTEXT_H_
