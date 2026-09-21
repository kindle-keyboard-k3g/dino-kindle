#ifndef DINO_GRAPHICS_SPRITE_RENDERER_H_
#define DINO_GRAPHICS_SPRITE_RENDERER_H_

#include "domain/cloud.h"
#include "domain/ground.h"
#include "domain/obstacle.h"
#include "domain/player.h"
#include "graphics/canvas.h"

namespace dino::graphics {

/**
 * @brief High-performance pixel art renderer for player, obstacles, and terrain.
 */
class SpriteRenderer {
public:
    static void draw_dino(Canvas& canvas,
                          int32_t x,
                          int32_t y,
                          domain::PlayerState state,
                          uint8_t leg_step,
                          domain::ColorValue color);

    static void draw_obstacle(Canvas& canvas,
                              const domain::Obstacle& obstacle,
                              uint8_t anim_step,
                              domain::ColorValue color);

    static void draw_cloud(Canvas& canvas,
                           const domain::Cloud& cloud,
                           domain::ColorValue color);

    static void draw_ground(Canvas& canvas,
                            const domain::Ground& ground,
                            int32_t screen_width,
                            domain::ColorValue color);
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_SPRITE_RENDERER_H_
