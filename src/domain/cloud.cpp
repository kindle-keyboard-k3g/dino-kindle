#include "domain/cloud.h"

namespace dino::domain {

ScreenCoordinate Cloud::position() const {
    return ScreenCoordinate(x_.to_pixels(), y_);
}

Dimensions Cloud::dimensions() const {
    return Dimensions(WIDTH, HEIGHT);
}

BoundingBox Cloud::visual_box() const {
    return BoundingBox(position(), dimensions());
}

bool Cloud::is_off_screen() const {
    const int32_t right_x = x_.to_pixels() + WIDTH;
    return right_x < 0;
}

void Cloud::update(FixedPoint speed) {
    x_ = x_ - speed;
}

}  // namespace dino::domain
