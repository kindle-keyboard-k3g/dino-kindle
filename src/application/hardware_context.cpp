#include "application/hardware_context.h"
#include "hal/eink_controller_mxc.h"
#include "hal/fallback_devices.h"
#include "hal/frame_buffer_fb0.h"
#include "hal/input_device_evdev.h"
#include "persistence/file_high_score_store.h"
#include "persistence/storage_path.h"

#include <unistd.h>

namespace dino::application {

namespace {

std::unique_ptr<persistence::HighScoreStore> create_score_store() {
    const std::string path = persistence::StoragePath::resolve_file_path("highscore.dat");
    return std::make_unique<persistence::FileHighScoreStore>(path);
}

std::unique_ptr<HardwareContext> create_host_context(bool is_ppm) {
    std::unique_ptr<hal::FrameBuffer> fb;
    std::unique_ptr<hal::InputDevice> input;
    if (is_ppm) {
        fb = std::make_unique<hal::PpmFrameBuffer>("dino.ppm");
        input = std::make_unique<hal::MockInputDevice>();
    } else {
        fb = std::make_unique<hal::AnsiTerminalFrameBuffer>();
        input = std::make_unique<hal::AnsiTerminalInput>();
    }
    auto epdc = std::make_unique<hal::MockEinkController>();
    DisplayBundle display(std::move(fb), std::move(epdc));
    PeripheralBundle periph(std::move(input), create_score_store());
    return std::make_unique<HardwareContext>(std::move(display), std::move(periph));
}

std::unique_ptr<HardwareContext> create_kindle_context() {
    try {
        auto fb = std::make_unique<hal::FrameBufferFb0>("/dev/fb0");
        auto epdc = std::make_unique<hal::EinkControllerMxc>("/dev/fb0");
        auto input = std::make_unique<hal::InputDeviceEvdev>();
        DisplayBundle display(std::move(fb), std::move(epdc));
        PeripheralBundle periph(std::move(input), create_score_store());
        return std::make_unique<HardwareContext>(std::move(display), std::move(periph));
    } catch (...) {
        return create_host_context(false);
    }
}

}  // namespace

std::unique_ptr<HardwareContext> HardwareContext::create(const ApplicationOptions& options) {
    if (options.is_ppm()) {
        return create_host_context(true);
    }
    if (options.is_ansi()) {
        return create_host_context(false);
    }
    if (::access("/dev/fb0", R_OK | W_OK) == 0) {
        return create_kindle_context();
    }
    return create_host_context(false);
}

}  // namespace dino::application
