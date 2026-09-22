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

    /// Downsamples and renders canvas to ANSI terminal with cursor positioning.
    void present(const graphics::Canvas& canvas,
                 const domain::BoundingBox& damage) override;
};

/**
 * @brief Serializes canvas frames to P6/P5 PPM binary image files.
 */
class PpmFrameBuffer : public FrameBuffer {
public:
    /// Constructs PPM framebuffer writing to specified path.
    explicit PpmFrameBuffer(std::string output_path = "output.ppm");

    /// Dumps canvas to binary PPM image.
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
    /// Enables raw non-canonical terminal mode.
    AnsiTerminalInput();
    /// Restores previous terminal attributes.
    ~AnsiTerminalInput() override;

    /// Reads pending ANSI key sequences into input state.
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

    /// Records present call and damages.
    void present(const graphics::Canvas& canvas,
                 const domain::BoundingBox& damage) override;

    /// Number of present calls.
    [[nodiscard]] size_t present_count() const { return present_count_; }
    /// Bounding box of most recent damage area.
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

    /// Records update call parameters.
    void update(const domain::BoundingBox& area,
                graphics::RefreshWaveform waveform) override;

    /// Number of update calls.
    [[nodiscard]] size_t update_count() const { return update_count_; }
    /// Waveform from most recent update call.
    [[nodiscard]] graphics::RefreshWaveform last_waveform() const { return last_waveform_; }
    /// Damaged area from most recent update call.
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

    /// Enqueues jump action for next poll.
    void queue_jump();
    /// Enqueues duck start action for next poll.
    void queue_duck_start();
    /// Enqueues duck stop action for next poll.
    void queue_duck_stop();
    /// Enqueues restart action for next poll.
    void queue_restart();
    /// Enqueues quit action for next poll.
    void queue_quit();

    /// Drains queued actions into input state.
    void poll_events(game::InputState& state) override;

private:
    std::vector<game::InputAction> queued_actions_;
};

}  // namespace dino::hal

#endif  // DINO_HAL_FALLBACK_DEVICES_H_
