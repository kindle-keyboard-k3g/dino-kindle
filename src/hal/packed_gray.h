#ifndef DINO_HAL_PACKED_GRAY_H_
#define DINO_HAL_PACKED_GRAY_H_

#include "domain/geometry.h"
#include "graphics/canvas.h"

#include <cstddef>
#include <cstdint>

namespace dino::hal {

/**
 * @brief Translates 8bpp canvas rasters into Kindle 4bpp packed nibble format.
 */
class PackedGray {
public:
    static constexpr size_t KINDLE_FB_SIZE = 240000; // 600 * 800 / 2
    static constexpr int32_t ROW_BYTES = 300;        // 600 / 2

    static void pack_frame(const graphics::Canvas& canvas, uint8_t* destination);

    static void pack_region(const graphics::Canvas& canvas,
                            const domain::BoundingBox& region,
                            uint8_t* destination);

    [[nodiscard]] static uint8_t pack_pixel_pair(uint8_t left_pixel,
                                                 uint8_t right_pixel);
};

}  // namespace dino::hal

#endif  // DINO_HAL_PACKED_GRAY_H_
