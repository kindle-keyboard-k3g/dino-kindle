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
    SceneRenderer() : animation_tick_(0), debug_overlay_(false) {}

    /**
     * @brief Clears canvas and renders environment, actors, HUD, and game over overlays.
     * @param canvas Target destination Canvas.
     * @param snapshot Game state snapshot containing entities and status.
     */
    void render(Canvas& canvas, const game::GameSnapshot& snapshot);

    /// Toggles debug hitbox and metrics overlay rendering.
    void set_debug_overlay(bool enabled) { debug_overlay_ = enabled; }

private:
    void render_hud(Canvas& canvas,
                    const game::RenderStatus& status,
                    domain::ColorValue fg_color);
    void render_game_over(Canvas& canvas, domain::ColorValue fg_color);
    void render_debug_overlay(Canvas& canvas,
                              const game::GameSnapshot& snapshot,
                              domain::ColorValue fg_color);

    uint8_t animation_tick_;
    bool debug_overlay_;
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_SCENE_RENDERER_H_
