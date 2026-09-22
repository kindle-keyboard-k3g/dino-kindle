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

    /**
     * @brief Renders a single 5x7 ASCII character onto canvas.
     * @param canvas Target Canvas.
     * @param x Left coordinate.
     * @param y Top coordinate.
     * @param character ASCII character to draw.
     * @param color Drawing color.
     */
    static void draw_char(Canvas& canvas,
                          int32_t x,
                          int32_t y,
                          char character,
                          domain::ColorValue color);

    /**
     * @brief Renders null-terminated ASCII string onto canvas.
     * @param canvas Target Canvas.
     * @param x Left coordinate.
     * @param y Top coordinate.
     * @param text ASCII string.
     * @param color Drawing color.
     */
    static void draw_text(Canvas& canvas,
                          int32_t x,
                          int32_t y,
                          const char* text,
                          domain::ColorValue color);

    /**
     * @brief Computes rendered pixel width of string including spacing.
     * @param text ASCII string to measure.
     * @return Width in pixels.
     */
    [[nodiscard]] static int32_t measure_width(const char* text);
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_BITMAP_FONT_H_
