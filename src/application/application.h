#ifndef DINO_APPLICATION_APPLICATION_H_
#define DINO_APPLICATION_APPLICATION_H_

#include "application/hardware_context.h"
#include "game/game_engine.h"
#include "game/input_state.h"
#include "graphics/canvas.h"
#include "graphics/refresh_coordinator.h"
#include "graphics/scene_renderer.h"
#include "util/monotonic_clock.h"

#include <memory>

namespace dino::application {

class EngineBundle {
public:
    explicit EngineBundle(uint32_t seed) : engine_(seed), input_{} {}

    [[nodiscard]] game::GameEngine& engine() { return engine_; }
    [[nodiscard]] game::InputState& input() { return input_; }

private:
    game::GameEngine engine_;
    game::InputState input_;
};

class PipelineBundle {
public:
    PipelineBundle() = default;

    [[nodiscard]] graphics::SceneRenderer& renderer() { return renderer_; }
    [[nodiscard]] graphics::RefreshCoordinator& refresh() { return refresh_; }

private:
    graphics::SceneRenderer renderer_;
    graphics::RefreshCoordinator refresh_;
};

class ApplicationBuffers {
public:
    ApplicationBuffers() = default;

    [[nodiscard]] graphics::Canvas& front() { return front_; }
    [[nodiscard]] graphics::Canvas& back() { return back_; }
    void copy_front_to_back();

private:
    graphics::Canvas front_;
    graphics::Canvas back_;
};

class ApplicationServices {
public:
    explicit ApplicationServices(std::unique_ptr<HardwareContext> hw)
        : hardware_(std::move(hw)), clock_{} {}

    [[nodiscard]] HardwareContext& hardware() { return *hardware_; }
    [[nodiscard]] const util::MonotonicClock& clock() const { return clock_; }

private:
    std::unique_ptr<HardwareContext> hardware_;
    util::MonotonicClock clock_;
};

class ApplicationRuntime {
public:
    explicit ApplicationRuntime(std::unique_ptr<HardwareContext> hw)
        : buffers_{}, services_(std::move(hw)) {}

    [[nodiscard]] ApplicationBuffers& buffers() { return buffers_; }
    [[nodiscard]] ApplicationServices& services() { return services_; }

private:
    ApplicationBuffers buffers_;
    ApplicationServices services_;
};

class FrameLimiter {
public:
    explicit FrameLimiter(int32_t max_frames)
        : max_frames_(max_frames), frame_count_(0) {}

    [[nodiscard]] bool is_limit_reached() const {
        if (max_frames_ < 0) {
            return false;
        }
        return frame_count_ >= max_frames_;
    }

    void advance() { frame_count_++; }

private:
    int32_t max_frames_;
    int32_t frame_count_;
};

class ApplicationExecution {
public:
    ApplicationExecution(std::unique_ptr<HardwareContext> hw, int32_t max_frames)
        : runtime_(std::move(hw)), limiter_(max_frames) {}

    [[nodiscard]] ApplicationRuntime& runtime() { return runtime_; }
    [[nodiscard]] FrameLimiter& limiter() { return limiter_; }

private:
    ApplicationRuntime runtime_;
    FrameLimiter limiter_;
};

class ApplicationCore {
public:
    explicit ApplicationCore(uint32_t seed)
        : engine_bundle_(seed), pipeline_bundle_{} {}

    [[nodiscard]] EngineBundle& engine_bundle() { return engine_bundle_; }
    [[nodiscard]] PipelineBundle& pipeline_bundle() { return pipeline_bundle_; }

private:
    EngineBundle engine_bundle_;
    PipelineBundle pipeline_bundle_;
};

/**
 * @brief Master application controller driving the 12.5 FPS fixed-step loop.
 */
class Application {
public:
    /// Constructs application with parsed CLI options.
    explicit Application(const ApplicationOptions& options);

    /**
     * @brief Executes main 80ms loop until shutdown signal or max frames.
     * @return Process exit code (0 on normal exit).
     */
    int run();

private:
    void step_frame();
    void present_and_refresh();
    void sync_frame_rate(util::Milliseconds frame_start);

    ApplicationCore core_;
    ApplicationExecution execution_;
};

}  // namespace dino::application

#endif  // DINO_APPLICATION_APPLICATION_H_
