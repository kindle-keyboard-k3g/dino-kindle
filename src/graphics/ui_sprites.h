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
    static SpriteMask game_over_banner();
    static SpriteMask restart_button();
    static SpriteMask score_digit(int32_t digit);
    static SpriteMask score_hi();
    static const GroundBump* ground_bumps();
    static size_t ground_bump_count();
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_UI_SPRITES_H_
