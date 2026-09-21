#ifndef DINO_HAL_FRAME_BUFFER_FB0_H_
#define DINO_HAL_FRAME_BUFFER_FB0_H_

#include "hal/frame_buffer.h"

#include <cstdint>

namespace dino::hal {

/**
 * @brief Direct memory-mapped /dev/fb0 Kindle hardware framebuffer driver.
 */
class FrameBufferFb0 : public FrameBuffer {
public:
    explicit FrameBufferFb0(const char* device_path = "/dev/fb0");
    ~FrameBufferFb0() override;

    FrameBufferFb0(const FrameBufferFb0&) = delete;
    FrameBufferFb0& operator=(const FrameBufferFb0&) = delete;

    void present(const graphics::Canvas& canvas,
                 const domain::BoundingBox& damage) override;

private:
    int file_descriptor_;
    uint8_t* mapped_memory_;
};

}  // namespace dino::hal

#endif  // DINO_HAL_FRAME_BUFFER_FB0_H_
