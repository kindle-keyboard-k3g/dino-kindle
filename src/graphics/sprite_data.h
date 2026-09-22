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
    /// Dino standing / jumping pose (44x48).
    static SpriteMask dino_standing();
    /// Dino run left leg step (44x48).
    static SpriteMask dino_run_left();
    /// Dino run right leg step (44x48).
    static SpriteMask dino_run_right();
    /// Dino duck left leg step (56x28).
    static SpriteMask dino_duck_left();
    /// Dino duck right leg step (56x28).
    static SpriteMask dino_duck_right();
    /// Dino crashed collision pose (44x48).
    static SpriteMask dino_crashed();
    /// Small cactus obstacle (20x36).
    static SpriteMask small_cactus();
    /// Large cactus obstacle (32x48).
    static SpriteMask large_cactus();
    /// Pterodactyl wing up frame (42x32).
    static SpriteMask ptero_wing_up();
    /// Pterodactyl wing down frame (42x32).
    static SpriteMask ptero_wing_down();
    /// Decorative drifting cloud (46x14).
    static SpriteMask cloud();
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_SPRITE_DATA_H_
