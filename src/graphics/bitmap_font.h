#ifndef DINO_GRAPHICS_BITMAP_FONT_H_
#define DINO_GRAPHICS_BITMAP_FONT_H_

#include "graphics/canvas.h"

namespace dino::graphics {

/**
 * @brief Compact 5x7 monospaced bitmap font for HUD and score overlays.
 */
class BitmapFont {
public:
    static constexpr int32_t GLYPH_WIDTH = 5;
    static constexpr int32_t GLYPH_HEIGHT = 7;
    static constexpr int32_t CHAR_SPACING = 2;

    static void draw_char(Canvas& canvas,
                          int32_t x,
                          int32_t y,
                          char character,
                          domain::ColorValue color);

    static void draw_text(Canvas& canvas,
                          int32_t x,
                          int32_t y,
                          const char* text,
                          domain::ColorValue color);

    [[nodiscard]] static int32_t measure_width(const char* text);
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_BITMAP_FONT_H_
