#ifndef DINO_GRAPHICS_FRAME_DIFFER_H_
#define DINO_GRAPHICS_FRAME_DIFFER_H_

#include "domain/geometry.h"
#include "graphics/canvas.h"

namespace dino::graphics {

/**
 * @brief Computes damage bounding boxes by diffing front and back raster buffers.
 */
class FrameDiffer {
public:
    [[nodiscard]] static domain::BoundingBox compute_damage(
        const Canvas& previous_frame,
        const Canvas& current_frame);
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_FRAME_DIFFER_H_
