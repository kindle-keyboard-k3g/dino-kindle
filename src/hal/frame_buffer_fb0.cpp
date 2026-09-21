#include "hal/frame_buffer_fb0.h"
#include "hal/packed_gray.h"

#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

namespace dino::hal {

FrameBufferFb0::FrameBufferFb0(const char* device_path)
    : file_descriptor_(-1), mapped_memory_(nullptr) {
    file_descriptor_ = ::open(device_path, O_RDWR);
    if (file_descriptor_ < 0) {
        throw std::runtime_error("Failed to open framebuffer device");
    }
    void* ptr = ::mmap(nullptr, PackedGray::KINDLE_FB_SIZE,
                       PROT_READ | PROT_WRITE, MAP_SHARED,
                       file_descriptor_, 0);
    if (ptr == MAP_FAILED) {
        ::close(file_descriptor_);
        throw std::runtime_error("Failed to mmap framebuffer memory");
    }
    mapped_memory_ = static_cast<uint8_t*>(ptr);
}

FrameBufferFb0::~FrameBufferFb0() {
    if (mapped_memory_ != nullptr && mapped_memory_ != MAP_FAILED) {
        ::munmap(mapped_memory_, PackedGray::KINDLE_FB_SIZE);
    }
    if (file_descriptor_ >= 0) {
        ::close(file_descriptor_);
    }
}

void FrameBufferFb0::present(const graphics::Canvas& canvas,
                             const domain::BoundingBox& damage) {
    if (damage.is_empty() || mapped_memory_ == nullptr) {
        return;
    }
    PackedGray::pack_region(canvas, damage, mapped_memory_);
}

}  // namespace dino::hal
