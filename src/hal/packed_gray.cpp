#include "hal/packed_gray.h"

#include <algorithm>

namespace dino::hal {

uint8_t PackedGray::pack_pixel_pair(uint8_t left_pixel, uint8_t right_pixel) {
    const uint8_t left_nibble = static_cast<uint8_t>((left_pixel & 0x0F) << 4);
    const uint8_t right_nibble = static_cast<uint8_t>(right_pixel & 0x0F);
    return static_cast<uint8_t>(left_nibble | right_nibble);
}

void PackedGray::pack_frame(const graphics::Canvas& canvas, uint8_t* destination) {
    const uint8_t* src = canvas.buffer();
    const size_t total_pixels = canvas.size();
    size_t dest_idx = 0;
    for (size_t i = 0; i + 1 < total_pixels; i += 2) {
        destination[dest_idx++] = pack_pixel_pair(src[i], src[i + 1]);
    }
}

void PackedGray::pack_region(const graphics::Canvas& canvas,
                             const domain::BoundingBox& region,
                             uint8_t* destination) {
    if (region.is_empty()) {
        return;
    }
    const int32_t width = canvas.dimensions().width();
    const int32_t height = canvas.dimensions().height();
    const int32_t left_even = std::max(0, region.left() & ~1);
    const int32_t right_even = std::min(width, (region.right() + 1) & ~1);
    const int32_t top = std::max(0, region.top());
    const int32_t bottom = std::min(height, region.bottom());
    const uint8_t* src = canvas.buffer();

    for (int32_t y = top; y < bottom; ++y) {
        const size_t src_row_offset = static_cast<size_t>(y * width);
        const size_t dest_row_offset = static_cast<size_t>(y * ROW_BYTES);
        for (int32_t x = left_even; x < right_even; x += 2) {
            const uint8_t p0 = src[src_row_offset + x];
            const uint8_t p1 = src[src_row_offset + x + 1];
            destination[dest_row_offset + (x / 2)] = pack_pixel_pair(p0, p1);
        }
    }
}

}  // namespace dino::hal
