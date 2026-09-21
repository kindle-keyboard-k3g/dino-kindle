#include "graphics/scene_renderer.h"
#include "graphics/bitmap_font.h"
#include "graphics/sprite_renderer.h"

#include <cstdio>

namespace dino::graphics {

void SceneRenderer::render_hud(Canvas& canvas,
                               const game::RenderStatus& status,
                               domain::ColorValue fg_color) {
    char score_str[32];
    std::snprintf(score_str, sizeof(score_str), "HI %05d  %05d",
                  status.high_score().points(),
                  status.current_score().points());
    const int32_t text_w = BitmapFont::measure_width(score_str);
    const int32_t x = canvas.dimensions().width() - text_w - 20;
    BitmapFont::draw_text(canvas, x, 30, score_str, fg_color);
}

void SceneRenderer::render_game_over(Canvas& canvas, domain::ColorValue fg_color) {
    const char* title = "G A M E   O V E R";
    const int32_t title_w = BitmapFont::measure_width(title);
    const int32_t center_x = (canvas.dimensions().width() - title_w) / 2;
    BitmapFont::draw_text(canvas, center_x, 260, title, fg_color);

    const char* sub = "PRESS SPACE OR UP TO RESTART";
    const int32_t sub_w = BitmapFont::measure_width(sub);
    const int32_t sub_x = (canvas.dimensions().width() - sub_w) / 2;
    BitmapFont::draw_text(canvas, sub_x, 300, sub, fg_color);
}

void SceneRenderer::render(Canvas& canvas, const game::GameSnapshot& snapshot) {
    const auto& status = snapshot.status();
    const domain::ColorValue bg_color = status.is_night()
        ? domain::ColorValue::Black : domain::ColorValue::White;
    const domain::ColorValue fg_color = status.is_night()
        ? domain::ColorValue::White : domain::ColorValue::Black;

    canvas.clear(bg_color);
    animation_tick_ = static_cast<uint8_t>(1 - animation_tick_);

    const auto& env = snapshot.world().environment();
    for (size_t i = 0; i < env.clouds().count(); ++i) {
        SpriteRenderer::draw_cloud(canvas, env.clouds().at(i), fg_color);
    }
    SpriteRenderer::draw_ground(canvas, env.ground(), canvas.dimensions().width(), fg_color);

    const auto& actors = snapshot.world().actors();
    for (size_t i = 0; i < actors.obstacles().count(); ++i) {
        SpriteRenderer::draw_obstacle(canvas, actors.obstacles().at(i), animation_tick_, fg_color);
    }

    const auto& player = actors.player();
    const auto pos = player.position();
    SpriteRenderer::draw_dino(canvas, pos.x(), pos.y(), player.state(), player.leg_step(), fg_color);

    render_hud(canvas, status, fg_color);
    if (status.is_game_over()) {
        render_game_over(canvas, fg_color);
    }
}

}  // namespace dino::graphics
