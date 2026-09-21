#include "graphics/bitmap_font.h"

#include <cctype>
#include <cstring>

namespace dino::graphics {

namespace {

// 7 rows of 5 bits each (stored in upper 5 bits: bit 7 = col 0, bit 3 = col 4).
const uint8_t* get_glyph_rows(char c) {
    static const uint8_t DIGITS[10][7] = {
        {0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70}, // 0
        {0x20, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70}, // 1
        {0x70, 0x88, 0x08, 0x10, 0x20, 0x40, 0xF8}, // 2
        {0x70, 0x88, 0x08, 0x30, 0x08, 0x88, 0x70}, // 3
        {0x10, 0x30, 0x50, 0x90, 0xF8, 0x10, 0x10}, // 4
        {0xF8, 0x80, 0xF0, 0x08, 0x08, 0x88, 0x70}, // 5
        {0x30, 0x40, 0x80, 0xF0, 0x88, 0x88, 0x70}, // 6
        {0xF8, 0x08, 0x10, 0x20, 0x40, 0x40, 0x40}, // 7
        {0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70}, // 8
        {0x70, 0x88, 0x88, 0x78, 0x08, 0x10, 0x60}  // 9
    };
    static const uint8_t SPACE[7] = {0, 0, 0, 0, 0, 0, 0};
    static const uint8_t DASH[7] = {0, 0, 0, 0xF8, 0, 0, 0};
    static const uint8_t DEFAULT_CHAR[7] = {
        0xF8, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88  // Default 'A'
    };

    if (c >= '0' && c <= '9') {
        return DIGITS[c - '0'];
    }
    if (c == '-') {
        return DASH;
    }
    if (c == ' ') {
        return SPACE;
    }
    return DEFAULT_CHAR;
}

void render_glyph_rows(Canvas& canvas,
                       int32_t x,
                       int32_t y,
                       const uint8_t* rows,
                       domain::ColorValue color) {
    for (int32_t r = 0; r < 7; ++r) {
        const uint8_t row_bits = rows[r];
        for (int32_t c = 0; c < 5; ++c) {
            const bool bit_set = (row_bits & (0x80 >> c)) != 0;
            if (bit_set) {
                canvas.set_pixel(x + c, y + r, color);
            }
        }
    }
}

}  // namespace

void BitmapFont::draw_char(Canvas& canvas,
                           int32_t x,
                           int32_t y,
                           char character,
                           domain::ColorValue color) {
    const uint8_t* rows = get_glyph_rows(character);
    render_glyph_rows(canvas, x, y, rows, color);
}

void BitmapFont::draw_text(Canvas& canvas,
                           int32_t x,
                           int32_t y,
                           const char* text,
                           domain::ColorValue color) {
    if (text == nullptr) {
        return;
    }
    int32_t cur_x = x;
    const size_t len = std::strlen(text);
    for (size_t i = 0; i < len; ++i) {
        draw_char(canvas, cur_x, y, text[i], color);
        cur_x += GLYPH_WIDTH + CHAR_SPACING;
    }
}

int32_t BitmapFont::measure_width(const char* text) {
    if (text == nullptr) {
        return 0;
    }
    const size_t len = std::strlen(text);
    if (len == 0) {
        return 0;
    }
    return static_cast<int32_t>(len * (GLYPH_WIDTH + CHAR_SPACING) - CHAR_SPACING);
}

}  // namespace dino::graphics
