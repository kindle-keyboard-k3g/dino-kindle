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

    [[nodiscard]] constexpr int32_t baseline_y() const { return baseline_y_; }
    [[nodiscard]] int32_t offset_pixels() const;
    [[nodiscard]] BoundingBox visual_box(int32_t screen_width) const;

    void update(FixedPoint speed);
    void reset();

private:
    FixedPoint offset_;
    int32_t baseline_y_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_GROUND_H_
