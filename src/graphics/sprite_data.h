#ifndef DINO_GRAPHICS_SPRITE_DATA_H_
#define DINO_GRAPHICS_SPRITE_DATA_H_

#include <cstdint>

namespace dino::graphics {

/**
 * @brief 1-bit packed sprite data structures for Chrome Dino elements.
 */
struct SpriteMask {
    int32_t width;
    int32_t height;
    const uint8_t* data;
};

class SpriteData {
public:
    static SpriteMask dino_standing();
    static SpriteMask dino_run_left();
    static SpriteMask dino_run_right();
    static SpriteMask dino_duck_left();
    static SpriteMask dino_duck_right();
    static SpriteMask dino_crashed();
    static SpriteMask small_cactus();
    static SpriteMask large_cactus();
    static SpriteMask ptero_wing_up();
    static SpriteMask ptero_wing_down();
    static SpriteMask cloud();
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_SPRITE_DATA_H_
