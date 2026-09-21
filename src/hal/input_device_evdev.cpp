#include "hal/input_device_evdev.h"

#include <fcntl.h>
#include <linux/input.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace dino::hal {

namespace {

bool is_jump_code(uint16_t code) {
    return code == KEY_SPACE || code == KEY_UP || code == KEY_ENTER ||
           code == 194 || code == 193 || code == 109;
}

bool is_duck_code(uint16_t code) {
    return code == KEY_DOWN || code == KEY_D || code == 104 || code == 191;
}

bool is_quit_code(uint16_t code) {
    return code == KEY_ESC || code == KEY_Q || code == 158; // 158 is KEY_BACK
}

}  // namespace

InputDeviceEvdev::InputDeviceEvdev() : grab_active_(false) {
    open_devices();
}

InputDeviceEvdev::~InputDeviceEvdev() {
    close_devices();
}

void InputDeviceEvdev::open_devices() {
    const char* paths[] = {"/dev/input/event0", "/dev/input/event1", "/dev/input/event2"};
    for (const char* path : paths) {
        const int fd = ::open(path, O_RDONLY | O_NONBLOCK);
        if (fd >= 0) {
            ::ioctl(fd, EVIOCGRAB, 1);
            descriptors_.push_back(fd);
        }
    }
    grab_active_ = !descriptors_.empty();
}

void InputDeviceEvdev::close_devices() {
    for (int fd : descriptors_) {
        if (grab_active_) {
            ::ioctl(fd, EVIOCGRAB, 0);
        }
        ::close(fd);
    }
    descriptors_.clear();
    grab_active_ = false;
}

void InputDeviceEvdev::process_event(uint16_t code, int32_t value, game::InputState& state) {
    if (is_jump_code(code) && value == 1) {
        state.handle_jump_press();
        return;
    }
    if (is_duck_code(code) && value == 1) {
        state.handle_duck_press();
        return;
    }
    if (is_duck_code(code) && value == 0) {
        state.handle_duck_release();
        return;
    }
    if (is_quit_code(code) && value == 1) {
        state.handle_quit_press();
        return;
    }
    if (code == KEY_R && value == 1) {
        state.handle_restart_press();
    }
}

void InputDeviceEvdev::poll_events(game::InputState& state) {
    if (descriptors_.empty()) {
        return;
    }
    std::vector<pollfd> poll_fds(descriptors_.size());
    for (size_t i = 0; i < descriptors_.size(); ++i) {
        poll_fds[i].fd = descriptors_[i];
        poll_fds[i].events = POLLIN;
        poll_fds[i].revents = 0;
    }
    if (::poll(poll_fds.data(), poll_fds.size(), 0) <= 0) {
        return;
    }
    for (size_t i = 0; i < descriptors_.size(); ++i) {
        if ((poll_fds[i].revents & POLLIN) == 0) {
            continue;
        }
        input_event ev{};
        while (::read(descriptors_[i], &ev, sizeof(ev)) == sizeof(ev)) {
            if (ev.type == EV_KEY) {
                process_event(ev.code, ev.value, state);
            }
        }
    }
}

}  // namespace dino::hal
