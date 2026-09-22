#include "application/application.h"
#include "graphics/frame_differ.h"
#include "util/debug_log.h"
#include "util/signal_guard.h"

#include <algorithm>
#include <cstring>
#include <unistd.h>

namespace dino::application {

void ApplicationBuffers::copy_front_to_back() {
    std::memcpy(back_.buffer(), front_.buffer(), front_.size());
}

Application::Application(const ApplicationOptions& options)
    : core_(options.seed()),
      execution_(HardwareContext::create(options), options.max_frames()) {
    util::DebugLog::set_enabled(options.is_debug());
    core_.engine_bundle().engine().set_godmode(options.is_godmode());
    core_.pipeline_bundle().renderer().set_debug_overlay(options.is_debug());
    util::DebugLog::log("[DINO-APP] Initialized with debug=", options.is_debug(),
                        " godmode=", options.is_godmode());
    auto& store = execution_.runtime().services().hardware().peripherals().store();
    core_.engine_bundle().engine().set_high_score(store.load());
}

void Application::step_frame() {
    auto& periph = execution_.runtime().services().hardware().peripherals();
    auto& input = core_.engine_bundle().input();
    periph.input().poll_events(input);

    auto& refresh = core_.pipeline_bundle().refresh();
    if (refresh.is_frozen()) {
        refresh.decrement_freeze();
        input.consume_momentary_triggers();
        return;
    }
    auto& engine = core_.engine_bundle().engine();
    engine.step(input);
    input.consume_momentary_triggers();
    if (engine.is_game_over()) {
        refresh.notify_game_over();
        periph.store().save(engine.snapshot().status().high_score());
    }
}

void Application::present_and_refresh() {
    auto& buffers = execution_.runtime().buffers();
    auto& pipeline = core_.pipeline_bundle();
    auto& hw = execution_.runtime().services().hardware();

    const auto snapshot = core_.engine_bundle().engine().snapshot();
    pipeline.renderer().render(buffers.front(), snapshot);

    const auto waveform = pipeline.refresh().determine_waveform();
    domain::BoundingBox damage = graphics::FrameDiffer::compute_damage(
        buffers.back(), buffers.front());
    if (waveform == graphics::RefreshWaveform::FullGrayscaleClear || damage.is_empty()) {
        damage = domain::BoundingBox(
            domain::ScreenCoordinate(0, 0), buffers.front().dimensions());
    }
    hw.display().framebuffer().present(buffers.front(), damage);
    hw.display().eink().update(damage, waveform);
    pipeline.refresh().acknowledge_refresh();
    buffers.copy_front_to_back();
}

void Application::sync_frame_rate(util::Milliseconds frame_start) {
    const auto now = execution_.runtime().services().clock().now_milliseconds();
    const int64_t elapsed = now.count() - frame_start.count();
    const int64_t remaining = 80LL - elapsed;
    if (remaining > 0) {
        ::usleep(static_cast<useconds_t>(remaining * 1000LL));
    }
}

int Application::run() {
    while (!util::SignalGuard::is_interrupted() &&
           !core_.engine_bundle().input().is_quit_requested() &&
           !execution_.limiter().is_limit_reached()) {
        const auto frame_start = execution_.runtime().services().clock().now_milliseconds();
        step_frame();
        present_and_refresh();
        execution_.limiter().advance();
        sync_frame_rate(frame_start);
    }
    return 0;
}

}  // namespace dino::application
