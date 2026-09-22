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
    /**
     * @brief Computes minimum bounding box containing all modified pixels between frames.
     * @param previous_frame Previous front canvas.
     * @param current_frame Newly rendered back canvas.
     * @return BoundingBox of changed area, or empty box if identical.
     */
    [[nodiscard]] static domain::BoundingBox compute_damage(
        const Canvas& previous_frame,
        const Canvas& current_frame);
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_FRAME_DIFFER_H_
