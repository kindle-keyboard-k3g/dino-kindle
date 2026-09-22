#ifndef DINO_GRAPHICS_UI_SPRITES_H_
#define DINO_GRAPHICS_UI_SPRITES_H_

#include "graphics/sprite_data.h"
#include <cstddef>
#include <cstdint>

namespace dino::graphics {

struct GroundBump {
    int16_t x;
    int8_t y_offset;
    int8_t length;
};

/**
 * @brief Authentic Chrome Dino UI pixel art sprites (Game Over, Restart, HUD digits).
 */
class UiSprites {
public:
    /// Authentic 191x11 pixel "GAME OVER" banner sprite mask.
    static SpriteMask game_over_banner();
    /// Authentic 36x32 pixel circular restart button sprite mask.
    static SpriteMask restart_button();
    /// Authentic 10x13 pixel digital score digit sprite mask (0-9).
    static SpriteMask score_digit(int32_t digit);
    /// Authentic 20x13 pixel "HI" glyph sprite mask.
    static SpriteMask score_hi();
    /// Array of 44 authentic procedural ground terrain bumps.
    static const GroundBump* ground_bumps();
    /// Total count of ground bumps in the pattern array (44).
    static size_t ground_bump_count();
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_UI_SPRITES_H_
