#include "graphics/frame_differ.h"

#include <algorithm>

namespace dino::graphics {

namespace {

void update_bounds(int32_t x,
                   int32_t y,
                   int32_t& min_x,
                   int32_t& min_y,
                   int32_t& max_x,
                   int32_t& max_y) {
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
}

}  // namespace

domain::BoundingBox FrameDiffer::compute_damage(
    const Canvas& previous_frame,
    const Canvas& current_frame) {
    const int32_t width = current_frame.dimensions().width();
    const int32_t height = current_frame.dimensions().height();
    const uint8_t* prev_buf = previous_frame.buffer();
    const uint8_t* curr_buf = current_frame.buffer();

    int32_t min_x = width;
    int32_t min_y = height;
    int32_t max_x = -1;
    int32_t max_y = -1;

    for (int32_t y = 0; y < height; ++y) {
        const size_t row_offset = static_cast<size_t>(y * width);
        for (int32_t x = 0; x < width; ++x) {
            if (prev_buf[row_offset + x] != curr_buf[row_offset + x]) {
                update_bounds(x, y, min_x, min_y, max_x, max_y);
            }
        }
    }
    if (max_x < 0) {
        return domain::BoundingBox{};
    }
    return domain::BoundingBox(
        domain::ScreenCoordinate(min_x, min_y),
        domain::Dimensions(max_x - min_x + 1, max_y - min_y + 1));
}

}  // namespace dino::graphics
