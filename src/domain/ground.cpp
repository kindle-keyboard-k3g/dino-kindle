#include "domain/ground.h"

namespace dino::domain {

int32_t Ground::offset_pixels() const {
    const int32_t raw_pixels = offset_.to_pixels();
    return raw_pixels % PATTERN_REPEAT_WIDTH;
}

BoundingBox Ground::visual_box(int32_t screen_width) const {
    return BoundingBox(
        ScreenCoordinate(0, baseline_y_),
        Dimensions(screen_width, HEIGHT));
}

void Ground::update(FixedPoint speed) {
    offset_ = offset_ + speed;
    const FixedPoint repeat_threshold = FixedPoint::from_pixels(PATTERN_REPEAT_WIDTH);
    if (offset_ >= repeat_threshold) {
        offset_ = offset_ - repeat_threshold;
    }
}

void Ground::reset() {
    offset_ = FixedPoint::from_pixels(0);
}

}  // namespace dino::domain
