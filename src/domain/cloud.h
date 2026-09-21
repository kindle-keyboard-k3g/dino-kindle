#ifndef DINO_DOMAIN_CLOUD_H_
#define DINO_DOMAIN_CLOUD_H_

#include "domain/game_values.h"
#include "domain/geometry.h"

namespace dino::domain {

/**
 * @brief Parallax decorative cloud drifting across the sky.
 */
class Cloud {
public:
    static constexpr int32_t WIDTH = 46;
    static constexpr int32_t HEIGHT = 14;

    constexpr Cloud() : x_(FixedPoint::from_pixels(0)), y_(0) {}
    constexpr Cloud(FixedPoint start_x, int32_t y) : x_(start_x), y_(y) {}

    [[nodiscard]] ScreenCoordinate position() const;
    [[nodiscard]] Dimensions dimensions() const;
    [[nodiscard]] BoundingBox visual_box() const;
    [[nodiscard]] bool is_off_screen() const;

    void update(FixedPoint speed);

private:
    FixedPoint x_;
    int32_t y_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_CLOUD_H_
