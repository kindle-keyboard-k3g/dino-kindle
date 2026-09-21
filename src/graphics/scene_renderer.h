#ifndef DINO_GRAPHICS_SCENE_RENDERER_H_
#define DINO_GRAPHICS_SCENE_RENDERER_H_

#include "game/game_snapshot.h"
#include "graphics/canvas.h"

namespace dino::graphics {

/**
 * @brief Composites game snapshot layers onto the 8bpp raster canvas.
 */
class SceneRenderer {
public:
    SceneRenderer() : animation_tick_(0) {}

    void render(Canvas& canvas, const game::GameSnapshot& snapshot);

private:
    void render_hud(Canvas& canvas,
                    const game::RenderStatus& status,
                    domain::ColorValue fg_color);
    void render_game_over(Canvas& canvas, domain::ColorValue fg_color);

    uint8_t animation_tick_;
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_SCENE_RENDERER_H_
