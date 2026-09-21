#ifndef DINO_GRAPHICS_CANVAS_H_
#define DINO_GRAPHICS_CANVAS_H_

#include "domain/geometry.h"
#include "domain/theme.h"

#include <cstdint>
#include <vector>

namespace dino::graphics {

/**
 * @brief 2D 8bpp monochrome raster canvas with clipping and primitive drawing.
 */
class Canvas {
public:
    static constexpr int32_t WIDTH = 600;
    static constexpr int32_t HEIGHT = 800;

    explicit Canvas(domain::Dimensions dimensions = domain::Dimensions(WIDTH, HEIGHT));

    [[nodiscard]] domain::Dimensions dimensions() const { return dimensions_; }
    [[nodiscard]] const uint8_t* buffer() const { return pixels_.data(); }
    [[nodiscard]] uint8_t* buffer() { return pixels_.data(); }
    [[nodiscard]] size_t size() const { return pixels_.size(); }

    void clear(domain::ColorValue color);
    void set_pixel(int32_t x, int32_t y, domain::ColorValue color);
    [[nodiscard]] domain::ColorValue get_pixel(int32_t x, int32_t y) const;
    void fill_rect(const domain::BoundingBox& box, domain::ColorValue color);
    void draw_bitmap(int32_t x,
                     int32_t y,
                     int32_t width,
                     int32_t height,
                     const uint8_t* mask,
                     domain::ColorValue color);

private:
    [[nodiscard]] bool is_inside(int32_t x, int32_t y) const;

    std::vector<uint8_t> pixels_;
    domain::Dimensions dimensions_;
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_CANVAS_H_
