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

    /// Constructs canvas with dimensions, allocating contiguous byte buffer.
    explicit Canvas(domain::Dimensions dimensions = domain::Dimensions(WIDTH, HEIGHT));

    /// Discrete dimensions of the canvas.
    [[nodiscard]] domain::Dimensions dimensions() const { return dimensions_; }
    /// Read-only pointer to raw pixel buffer.
    [[nodiscard]] const uint8_t* buffer() const { return pixels_.data(); }
    /// Mutable pointer to raw pixel buffer.
    [[nodiscard]] uint8_t* buffer() { return pixels_.data(); }
    /// Total byte count of pixel buffer.
    [[nodiscard]] size_t size() const { return pixels_.size(); }

    /// Fills entire canvas with specified color value.
    void clear(domain::ColorValue color);
    /// Sets pixel at (x, y) if within canvas bounds.
    void set_pixel(int32_t x, int32_t y, domain::ColorValue color);
    /// Reads color value of pixel at (x, y).
    [[nodiscard]] domain::ColorValue get_pixel(int32_t x, int32_t y) const;
    /// Fills solid axis-aligned rectangle with clipping.
    void fill_rect(const domain::BoundingBox& box, domain::ColorValue color);
    /// Draws 1px outline rectangle without filling interior.
    void draw_rect_outline(const domain::BoundingBox& box, domain::ColorValue color);
    /// Blits 1-bit packed MSB-first bitmap mask with clipping.
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
