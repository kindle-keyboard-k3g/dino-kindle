#ifndef DINO_DOMAIN_GROUND_H_
#define DINO_DOMAIN_GROUND_H_

#include "domain/game_values.h"
#include "domain/geometry.h"

namespace dino::domain {

/**
 * @brief Procedural scrolling ground line and terrain texture.
 */
class Ground {
public:
    static constexpr int32_t DEFAULT_BASELINE_Y = 520;
    static constexpr int32_t HEIGHT = 24;
    static constexpr int32_t PATTERN_REPEAT_WIDTH = 600;

    constexpr Ground()
        : offset_(FixedPoint::from_pixels(0)),
          baseline_y_(DEFAULT_BASELINE_Y) {}

    /**
     * @brief Baseline vertical coordinate of the running surface.
     * @return Y pixel coordinate of ground line.
     */
    [[nodiscard]] constexpr int32_t baseline_y() const { return baseline_y_; }

    /**
     * @brief Discrete pixel offset for texture scroll wrapping.
     * @return Pixel offset within the repeat width.
     */
    [[nodiscard]] int32_t offset_pixels() const;

    /**
     * @brief Bounding box enclosing the visible ground line across the screen.
     * @param screen_width Total width of the display.
     * @return BoundingBox covering ground area.
     */
    [[nodiscard]] BoundingBox visual_box(int32_t screen_width) const;

    /**
     * @brief Advances ground scroll offset.
     * @param speed Scroll velocity in fixed-point.
     */
    void update(FixedPoint speed);

    /**
     * @brief Resets ground scroll offset to zero.
     */
    void reset();

private:
    FixedPoint offset_;
    int32_t baseline_y_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_GROUND_H_
