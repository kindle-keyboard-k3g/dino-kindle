#include "hal/fallback_devices.h"

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace dino::hal {

void AnsiTerminalFrameBuffer::present(const graphics::Canvas& canvas,
                                      [[maybe_unused]] const domain::BoundingBox& damage) {
    std::cout << "\033[H\033[?25l";
    const int32_t step_x = canvas.dimensions().width() / TERM_WIDTH;
    const int32_t step_y = canvas.dimensions().height() / TERM_HEIGHT;
    for (int32_t row = 0; row < TERM_HEIGHT; ++row) {
        for (int32_t col = 0; col < TERM_WIDTH; ++col) {
            const auto color = canvas.get_pixel(col * step_x, row * step_y);
            const bool is_dark = (color != domain::ColorValue::White);
            std::cout << (is_dark ? "#" : " ");
        }
        std::cout << '\n';
    }
    std::cout.flush();
}

PpmFrameBuffer::PpmFrameBuffer(std::string output_path)
    : output_path_(std::move(output_path)) {}

void PpmFrameBuffer::present(const graphics::Canvas& canvas,
                             [[maybe_unused]] const domain::BoundingBox& damage) {
    std::ofstream out(output_path_, std::ios::binary);
    if (!out) {
        return;
    }
    const int32_t w = canvas.dimensions().width();
    const int32_t h = canvas.dimensions().height();
    out << "P5\n" << w << " " << h << "\n255\n";
    const uint8_t* src = canvas.buffer();
    const size_t total = canvas.size();
    for (size_t i = 0; i < total; ++i) {
        const uint8_t gray = static_cast<uint8_t>(255 - (src[i] * 17));
        out.put(static_cast<char>(gray));
    }
}

AnsiTerminalInput::AnsiTerminalInput() : raw_mode_enabled_(false) {
    enable_raw_mode();
}

AnsiTerminalInput::~AnsiTerminalInput() {
    disable_raw_mode();
}

void AnsiTerminalInput::enable_raw_mode() {
    struct termios raw {};
    if (::tcgetattr(STDIN_FILENO, &raw) != 0) {
        return;
    }
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    ::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    raw_mode_enabled_ = true;
}

void AnsiTerminalInput::disable_raw_mode() {
    if (!raw_mode_enabled_) {
        return;
    }
    struct termios original {};
    if (::tcgetattr(STDIN_FILENO, &original) == 0) {
        original.c_lflag |= (ECHO | ICANON);
        ::tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
    }
    raw_mode_enabled_ = false;
}

void AnsiTerminalInput::poll_events(game::InputState& state) {
    char ch = 0;
    while (::read(STDIN_FILENO, &ch, 1) > 0) {
        if (ch == ' ' || ch == 'w') {
            state.handle_jump_press();
            continue;
        }
        if (ch == 's') {
            state.handle_duck_press();
            continue;
        }
        if (ch == 'q') {
            state.handle_quit_press();
            continue;
        }
        if (ch == 'r') {
            state.handle_restart_press();
        }
    }
}

void MockFrameBuffer::present([[maybe_unused]] const graphics::Canvas& canvas,
                              const domain::BoundingBox& damage) {
    present_count_++;
    last_damage_ = damage;
}

void MockEinkController::update(const domain::BoundingBox& area,
                                graphics::RefreshWaveform waveform) {
    update_count_++;
    last_waveform_ = waveform;
    last_area_ = area;
}

void MockInputDevice::queue_jump() {
    queued_actions_.push_back(game::InputAction::Jump);
}
void MockInputDevice::queue_duck_start() {
    queued_actions_.push_back(game::InputAction::DuckStart);
}
void MockInputDevice::queue_duck_stop() {
    queued_actions_.push_back(game::InputAction::DuckStop);
}
void MockInputDevice::queue_restart() {
    queued_actions_.push_back(game::InputAction::Restart);
}
void MockInputDevice::queue_quit() {
    queued_actions_.push_back(game::InputAction::Quit);
}

void MockInputDevice::poll_events(game::InputState& state) {
    for (auto action : queued_actions_) {
        if (action == game::InputAction::Jump) {
            state.handle_jump_press();
        }
        if (action == game::InputAction::DuckStart) {
            state.handle_duck_press();
        }
        if (action == game::InputAction::DuckStop) {
            state.handle_duck_release();
        }
        if (action == game::InputAction::Restart) {
            state.handle_restart_press();
        }
        if (action == game::InputAction::Quit) {
            state.handle_quit_press();
        }
    }
    queued_actions_.clear();
}

}  // namespace dino::hal
