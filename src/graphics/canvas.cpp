#include "graphics/canvas.h"

#include <algorithm>

namespace dino::graphics {

Canvas::Canvas(domain::Dimensions dimensions)
    : pixels_(static_cast<size_t>(dimensions.width() * dimensions.height()),
              static_cast<uint8_t>(domain::ColorValue::White)),
      dimensions_(dimensions) {}

bool Canvas::is_inside(int32_t x, int32_t y) const {
    if (x < 0 || x >= dimensions_.width()) {
        return false;
    }
    if (y < 0 || y >= dimensions_.height()) {
        return false;
    }
    return true;
}

void Canvas::clear(domain::ColorValue color) {
    std::fill(pixels_.begin(), pixels_.end(), static_cast<uint8_t>(color));
}

void Canvas::set_pixel(int32_t x, int32_t y, domain::ColorValue color) {
    if (!is_inside(x, y)) {
        return;
    }
    const size_t index = static_cast<size_t>(y * dimensions_.width() + x);
    pixels_[index] = static_cast<uint8_t>(color);
}

domain::ColorValue Canvas::get_pixel(int32_t x, int32_t y) const {
    if (!is_inside(x, y)) {
        return domain::ColorValue::White;
    }
    const size_t index = static_cast<size_t>(y * dimensions_.width() + x);
    return static_cast<domain::ColorValue>(pixels_[index]);
}

void Canvas::fill_rect(const domain::BoundingBox& box, domain::ColorValue color) {
    const domain::BoundingBox screen_box(
        domain::ScreenCoordinate(0, 0), dimensions_);
    const domain::BoundingBox clipped = box.clamped_to(screen_box);
    if (clipped.is_empty()) {
        return;
    }
    for (int32_t y = clipped.top(); y < clipped.bottom(); ++y) {
        const size_t row_start = static_cast<size_t>(y * dimensions_.width() + clipped.left());
        const size_t span = static_cast<size_t>(clipped.width());
        std::fill_n(pixels_.data() + row_start, span, static_cast<uint8_t>(color));
    }
}

void Canvas::draw_rect_outline(const domain::BoundingBox& box, domain::ColorValue color) {
    if (box.is_empty()) {
        return;
    }
    const int32_t x1 = box.left();
    const int32_t x2 = box.right() - 1;
    const int32_t y1 = box.top();
    const int32_t y2 = box.bottom() - 1;
    for (int32_t x = x1; x <= x2; ++x) {
        set_pixel(x, y1, color);
        set_pixel(x, y2, color);
    }
    for (int32_t y = y1; y <= y2; ++y) {
        set_pixel(x1, y, color);
        set_pixel(x2, y, color);
    }
}

void Canvas::draw_bitmap(int32_t x,
                         int32_t y,
                         int32_t width,
                         int32_t height,
                         const uint8_t* mask,
                         domain::ColorValue color) {
    for (int32_t row = 0; row < height; ++row) {
        for (int32_t col = 0; col < width; ++col) {
            const size_t bit_index = static_cast<size_t>(row * width + col);
            const uint8_t byte_val = mask[bit_index / 8];
            const bool is_set = (byte_val & (0x80 >> (bit_index % 8))) != 0;
            if (is_set) {
                set_pixel(x + col, y + row, color);
            }
        }
    }
}

}  // namespace dino::graphics
