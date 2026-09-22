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
    /**
     * @brief Renders Dino sprite selected by pose state and leg animation step.
     * @param canvas Target destination Canvas.
     * @param x Left coordinate.
     * @param y Top coordinate.
     * @param state Active player pose.
     * @param leg_step Animation step index (0 or 1).
     * @param color Drawing color.
     */
    static void draw_dino(Canvas& canvas,
                          int32_t x,
                          int32_t y,
                          domain::PlayerState state,
                          uint8_t leg_step,
                          domain::ColorValue color);

    /**
     * @brief Renders obstacle sprite (cactus or pterodactyl).
     * @param canvas Target destination Canvas.
     * @param obstacle Obstacle entity to render.
     * @param anim_step Animation step for winged obstacles.
     * @param color Drawing color.
     */
    static void draw_obstacle(Canvas& canvas,
                              const domain::Obstacle& obstacle,
                              uint8_t anim_step,
                              domain::ColorValue color);

    /**
     * @brief Renders decorative cloud sprite.
     * @param canvas Target destination Canvas.
     * @param cloud Cloud entity to render.
     * @param color Drawing color.
     */
    static void draw_cloud(Canvas& canvas,
                           const domain::Cloud& cloud,
                           domain::ColorValue color);

    /**
     * @brief Renders horizontal baseline and procedural ground terrain bumps.
     * @param canvas Target destination Canvas.
     * @param ground Ground entity with baseline Y and scroll offset.
     * @param screen_width Canvas display width.
     * @param color Drawing color.
     */
    static void draw_ground(Canvas& canvas,
                            const domain::Ground& ground,
                            int32_t screen_width,
                            domain::ColorValue color);
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_SPRITE_RENDERER_H_
