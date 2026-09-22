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

    /**
     * @brief Computes discrete screen coordinate for rendering.
     * @return Screen coordinate of upper-left corner.
     */
    [[nodiscard]] ScreenCoordinate position() const;

    /**
     * @brief Returns dimensions of the cloud sprite.
     * @return Discrete Dimensions (width and height).
     */
    [[nodiscard]] Dimensions dimensions() const;

    /**
     * @brief Returns bounding box enclosing the visual sprite.
     * @return BoundingBox on screen.
     */
    [[nodiscard]] BoundingBox visual_box() const;

    /**
     * @brief Checks if cloud has drifted entirely past left screen edge.
     * @return True if off-screen to the left.
     */
    [[nodiscard]] bool is_off_screen() const;

    /**
     * @brief Advances cloud position to the left according to speed.
     * @param speed Current world scroll velocity in fixed-point.
     */
    void update(FixedPoint speed);

private:
    FixedPoint x_;
    int32_t y_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_CLOUD_H_
