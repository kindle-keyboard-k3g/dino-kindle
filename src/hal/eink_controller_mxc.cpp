#include "hal/eink_controller_mxc.h"

#include <fcntl.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>

namespace dino::hal {

namespace {

struct update_area_t {
    int x1, y1;
    int x2, y2;
    int which_fx;
    unsigned char* buffer;
};

}  // namespace

EinkControllerMxc::EinkControllerMxc(const char* device_path)
    : file_descriptor_(-1), owns_descriptor_(true) {
    file_descriptor_ = ::open(device_path, O_RDWR);
    if (file_descriptor_ < 0) {
        throw std::runtime_error("Failed to open epdc device for ioctl");
    }
}

EinkControllerMxc::EinkControllerMxc(int existing_fd)
    : file_descriptor_(existing_fd), owns_descriptor_(false) {
    if (file_descriptor_ < 0) {
        throw std::invalid_argument("Invalid file descriptor passed to EinkControllerMxc");
    }
}

EinkControllerMxc::~EinkControllerMxc() {
    if (owns_descriptor_ && file_descriptor_ >= 0) {
        ::close(file_descriptor_);
    }
}

void EinkControllerMxc::update(const domain::BoundingBox& area,
                               graphics::RefreshWaveform waveform) {
    if (area.is_empty() || file_descriptor_ < 0) {
        return;
    }
    update_area_t update_cmd{};
    update_cmd.x1 = area.left();
    update_cmd.y1 = area.top();
    update_cmd.x2 = area.right();
    update_cmd.y2 = area.bottom();
    update_cmd.which_fx = (waveform == graphics::RefreshWaveform::FullGrayscaleClear) ? 1 : 0;
    update_cmd.buffer = nullptr;

    ::ioctl(file_descriptor_, FBIO_EINK_UPDATE_DISPLAY_AREA, &update_cmd);
}

}  // namespace dino::hal
