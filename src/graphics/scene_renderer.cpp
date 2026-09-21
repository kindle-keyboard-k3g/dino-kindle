#include "graphics/scene_renderer.h"
#include "graphics/bitmap_font.h"
#include "graphics/sprite_renderer.h"
#include "graphics/ui_sprites.h"

namespace dino::graphics {

namespace {

void draw_5_digits(Canvas& canvas,
                   int32_t x,
                   int32_t y,
                   int32_t value,
                   domain::ColorValue color) {
    int32_t divisor = 10000;
    for (int32_t i = 0; i < 5; ++i) {
        const int32_t digit = (value / divisor) % 10;
        const SpriteMask mask = UiSprites::score_digit(digit);
        canvas.draw_bitmap(x + i * 11, y, mask.width, mask.height, mask.data, color);
        divisor /= 10;
    }
}

void render_environment(Canvas& canvas,
                        const domain::EnvironmentGroup& env,
                        domain::ColorValue fg_color) {
    for (size_t i = 0; i < env.clouds().count(); ++i) {
        SpriteRenderer::draw_cloud(canvas, env.clouds().at(i), fg_color);
    }
    SpriteRenderer::draw_ground(canvas, env.ground(), canvas.dimensions().width(), fg_color);
}

void render_actors(Canvas& canvas,
                   const domain::ActorGroup& actors,
                   uint8_t anim_tick,
                   domain::ColorValue fg_color) {
    for (size_t i = 0; i < actors.obstacles().count(); ++i) {
        SpriteRenderer::draw_obstacle(canvas, actors.obstacles().at(i), anim_tick, fg_color);
    }
    const auto& player = actors.player();
    const auto pos = player.position();
    SpriteRenderer::draw_dino(canvas, pos.x(), pos.y(), player.state(), player.leg_step(), fg_color);
}

}  // namespace

void SceneRenderer::render_hud(Canvas& canvas,
                               const game::RenderStatus& status,
                               domain::ColorValue fg_color) {
    const int32_t screen_w = canvas.dimensions().width();
    const int32_t hud_y = 25;
    const int32_t current_x = screen_w - 55 - 20;
    draw_5_digits(canvas, current_x, hud_y, status.current_score().points(), fg_color);

    const int32_t hi_x = current_x - 15 - 55 - 25;
    const SpriteMask hi_mask = UiSprites::score_hi();
    canvas.draw_bitmap(hi_x, hud_y, hi_mask.width, hi_mask.height, hi_mask.data, fg_color);
    draw_5_digits(canvas, hi_x + 25, hud_y, status.high_score().points(), fg_color);
}

void SceneRenderer::render_game_over(Canvas& canvas, domain::ColorValue fg_color) {
    const int32_t screen_w = canvas.dimensions().width();
    const SpriteMask banner = UiSprites::game_over_banner();
    const int32_t banner_x = (screen_w - banner.width) / 2;
    canvas.draw_bitmap(banner_x, 240, banner.width, banner.height, banner.data, fg_color);

    const SpriteMask restart = UiSprites::restart_button();
    const int32_t restart_x = (screen_w - restart.width) / 2;
    canvas.draw_bitmap(restart_x, 270, restart.width, restart.height, restart.data, fg_color);

    const char* hint = "[SPACE / UP]";
    const int32_t hint_w = BitmapFont::measure_width(hint);
    const int32_t hint_x = (screen_w - hint_w) / 2;
    BitmapFont::draw_text(canvas, hint_x, 320, hint, fg_color);
}

void SceneRenderer::render(Canvas& canvas, const game::GameSnapshot& snapshot) {
    const auto& status = snapshot.status();
    const domain::ColorValue bg_color = status.is_night()
        ? domain::ColorValue::Black : domain::ColorValue::White;
    const domain::ColorValue fg_color = status.is_night()
        ? domain::ColorValue::White : domain::ColorValue::Black;

    canvas.clear(bg_color);
    animation_tick_ = static_cast<uint8_t>(1 - animation_tick_);

    render_environment(canvas, snapshot.world().environment(), fg_color);
    render_actors(canvas, snapshot.world().actors(), animation_tick_, fg_color);
    render_hud(canvas, status, fg_color);
    if (status.is_game_over()) {
        render_game_over(canvas, fg_color);
    }
}

}  // namespace dino::graphics
