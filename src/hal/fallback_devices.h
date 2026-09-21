#ifndef DINO_HAL_FALLBACK_DEVICES_H_
#define DINO_HAL_FALLBACK_DEVICES_H_

#include "hal/eink_controller.h"
#include "hal/frame_buffer.h"
#include "hal/input_device.h"

#include <string>
#include <vector>

namespace dino::hal {

/**
 * @brief Downsamples canvas to ANSI terminal graphics for local desktop execution.
 */
class AnsiTerminalFrameBuffer : public FrameBuffer {
public:
    static constexpr int32_t TERM_WIDTH = 75;
    static constexpr int32_t TERM_HEIGHT = 40;

    void present(const graphics::Canvas& canvas,
                 const domain::BoundingBox& damage) override;
};

/**
 * @brief Serializes canvas frames to P6/P5 PPM binary image files.
 */
class PpmFrameBuffer : public FrameBuffer {
public:
    explicit PpmFrameBuffer(std::string output_path = "output.ppm");

    void present(const graphics::Canvas& canvas,
                 const domain::BoundingBox& damage) override;

private:
    std::string output_path_;
};

/**
 * @brief Non-blocking terminal keyboard polling for desktop execution.
 */
class AnsiTerminalInput : public InputDevice {
public:
    AnsiTerminalInput();
    ~AnsiTerminalInput() override;

    void poll_events(game::InputState& state) override;

private:
    void enable_raw_mode();
    void disable_raw_mode();

    bool raw_mode_enabled_;
};

/**
 * @brief Mock FrameBuffer for test verification.
 */
class MockFrameBuffer : public FrameBuffer {
public:
    MockFrameBuffer() : present_count_(0), last_damage_{} {}

    void present(const graphics::Canvas& canvas,
                 const domain::BoundingBox& damage) override;

    [[nodiscard]] size_t present_count() const { return present_count_; }
    [[nodiscard]] const domain::BoundingBox& last_damage() const { return last_damage_; }

private:
    size_t present_count_;
    domain::BoundingBox last_damage_;
};

/**
 * @brief Mock E-Ink controller for test verification.
 */
class MockEinkController : public EinkController {
public:
    MockEinkController()
        : update_count_(0), last_waveform_(graphics::RefreshWaveform::PartialDirectUpdate) {}

    void update(const domain::BoundingBox& area,
                graphics::RefreshWaveform waveform) override;

    [[nodiscard]] size_t update_count() const { return update_count_; }
    [[nodiscard]] graphics::RefreshWaveform last_waveform() const { return last_waveform_; }
    [[nodiscard]] const domain::BoundingBox& last_area() const { return last_area_; }

private:
    size_t update_count_;
    graphics::RefreshWaveform last_waveform_;
    domain::BoundingBox last_area_;
};

/**
 * @brief Mock InputDevice to inject deterministic actions in unit tests.
 */
class MockInputDevice : public InputDevice {
public:
    MockInputDevice() = default;

    void queue_jump();
    void queue_duck_start();
    void queue_duck_stop();
    void queue_restart();
    void queue_quit();

    void poll_events(game::InputState& state) override;

private:
    std::vector<game::InputAction> queued_actions_;
};

}  // namespace dino::hal

#endif  // DINO_HAL_FALLBACK_DEVICES_H_
