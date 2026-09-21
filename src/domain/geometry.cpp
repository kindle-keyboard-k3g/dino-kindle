#include "domain/geometry.h"

#include <algorithm>

namespace dino::domain {

bool BoundingBox::intersects(const BoundingBox& other) const {
    if (is_empty() || other.is_empty()) {
        return false;
    }
    if (right() <= other.left() || other.right() <= left()) {
        return false;
    }
    if (bottom() <= other.top() || other.bottom() <= top()) {
        return false;
    }
    return true;
}

BoundingBox BoundingBox::expanded(int32_t margin) const {
    if (is_empty()) {
        return *this;
    }
    const int32_t new_x = left() - margin;
    const int32_t new_y = top() - margin;
    const int32_t new_width = width() + (margin * 2);
    const int32_t new_height = height() + (margin * 2);
    return BoundingBox(ScreenCoordinate(new_x, new_y),
                       Dimensions(new_width, new_height));
}

BoundingBox BoundingBox::inset(int32_t margin) const {
    if (is_empty()) {
        return *this;
    }
    const int32_t new_width = width() - (margin * 2);
    const int32_t new_height = height() - (margin * 2);
    if (new_width <= 0 || new_height <= 0) {
        return BoundingBox{};
    }
    const int32_t new_x = left() + margin;
    const int32_t new_y = top() + margin;
    return BoundingBox(ScreenCoordinate(new_x, new_y),
                       Dimensions(new_width, new_height));
}

BoundingBox BoundingBox::merged_with(const BoundingBox& other) const {
    if (is_empty()) {
        return other;
    }
    if (other.is_empty()) {
        return *this;
    }
    const int32_t min_x = std::min(left(), other.left());
    const int32_t min_y = std::min(top(), other.top());
    const int32_t max_x = std::max(right(), other.right());
    const int32_t max_y = std::max(bottom(), other.bottom());
    return BoundingBox(ScreenCoordinate(min_x, min_y),
                       Dimensions(max_x - min_x, max_y - min_y));
}

BoundingBox BoundingBox::clamped_to(const BoundingBox& bounds) const {
    if (is_empty() || bounds.is_empty()) {
        return BoundingBox{};
    }
    const int32_t clamped_left = std::max(left(), bounds.left());
    const int32_t clamped_top = std::max(top(), bounds.top());
    const int32_t clamped_right = std::min(right(), bounds.right());
    const int32_t clamped_bottom = std::min(bottom(), bounds.bottom());
    if (clamped_right <= clamped_left || clamped_bottom <= clamped_top) {
        return BoundingBox{};
    }
    return BoundingBox(
        ScreenCoordinate(clamped_left, clamped_top),
        Dimensions(clamped_right - clamped_left, clamped_bottom - clamped_top));
}

}  // namespace dino::domain
