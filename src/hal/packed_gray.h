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

    /**
     * @brief Packs an entire 600x800 canvas into 4bpp destination buffer.
     * @param canvas Source Canvas.
     * @param destination Destination pointer (must hold at least 240,000 bytes).
     */
    static void pack_frame(const graphics::Canvas& canvas, uint8_t* destination);

    /**
     * @brief Packs only rows and columns bounded by region into destination.
     * @param canvas Source Canvas.
     * @param region Damaged bounding box.
     * @param destination Destination framebuffer memory.
     */
    static void pack_region(const graphics::Canvas& canvas,
                            const domain::BoundingBox& region,
                            uint8_t* destination);

    /**
     * @brief Combines two 4-bit pixel values into a single byte.
     * @param left_pixel Most-significant nibble (even pixel).
     * @param right_pixel Least-significant nibble (odd pixel).
     * @return Packed byte containing (left << 4) | right.
     */
    [[nodiscard]] static uint8_t pack_pixel_pair(uint8_t left_pixel,
                                                 uint8_t right_pixel);
};

}  // namespace dino::hal

#endif  // DINO_HAL_PACKED_GRAY_H_
