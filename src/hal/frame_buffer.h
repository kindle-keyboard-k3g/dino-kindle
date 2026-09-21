#ifndef DINO_HAL_FRAME_BUFFER_H_
#define DINO_HAL_FRAME_BUFFER_H_

#include "domain/geometry.h"
#include "graphics/canvas.h"

namespace dino::hal {

/**
 * @brief Abstract display surface destination.
 */
class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    /**
     * @brief Writes damaged canvas regions to the underlying physical display buffer.
     */
    virtual void present(const graphics::Canvas& canvas,
                         const domain::BoundingBox& damage) = 0;
};

}  // namespace dino::hal

#endif  // DINO_HAL_FRAME_BUFFER_H_
