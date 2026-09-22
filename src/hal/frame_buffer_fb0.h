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
    /**
     * @brief Opens and memory-maps /dev/fb0 into process address space.
     * @param device_path Path to framebuffer device node.
     */
    explicit FrameBufferFb0(const char* device_path = "/dev/fb0");
    ~FrameBufferFb0() override;

    FrameBufferFb0(const FrameBufferFb0&) = delete;
    FrameBufferFb0& operator=(const FrameBufferFb0&) = delete;

    /**
     * @brief Packs and copies damaged canvas pixels into mapped framebuffer memory.
     * @param canvas Rendered source canvas.
     * @param damage Damaged bounding region.
     */
    void present(const graphics::Canvas& canvas,
                 const domain::BoundingBox& damage) override;

private:
    int file_descriptor_;
    uint8_t* mapped_memory_;
};

}  // namespace dino::hal

#endif  // DINO_HAL_FRAME_BUFFER_FB0_H_
