#include "graphics/sprite_renderer.h"
#include "graphics/sprite_data.h"
#include "graphics/ui_sprites.h"

namespace dino::graphics {

namespace {

SpriteMask pick_duck_mask(uint8_t leg_step) {
    if (leg_step == 0) {
        return SpriteData::dino_duck_left();
    }
    return SpriteData::dino_duck_right();
}

SpriteMask pick_run_mask(uint8_t leg_step) {
    if (leg_step == 0) {
        return SpriteData::dino_run_left();
    }
    return SpriteData::dino_run_right();
}

SpriteMask pick_dino_mask(domain::PlayerState state, uint8_t leg_step) {
    if (state == domain::PlayerState::Crashed) {
        return SpriteData::dino_crashed();
    }
    if (state == domain::PlayerState::Ducking) {
        return pick_duck_mask(leg_step);
    }
    if (state == domain::PlayerState::Jumping) {
        return SpriteData::dino_standing();
    }
    return pick_run_mask(leg_step);
}

SpriteMask pick_obstacle_mask(domain::ObstacleType type, uint8_t anim_step) {
    if (type == domain::ObstacleType::SmallCactus) {
        return SpriteData::small_cactus();
    }
    if (type == domain::ObstacleType::LargeCactus) {
        return SpriteData::large_cactus();
    }
    if (anim_step == 0) {
        return SpriteData::ptero_wing_up();
    }
    return SpriteData::ptero_wing_down();
}

void draw_single_bump(Canvas& canvas,
                      int32_t screen_x,
                      int32_t y,
                      int32_t length,
                      int32_t screen_width,
                      domain::ColorValue color) {
    for (int32_t dx = 0; dx < length; ++dx) {
        const int32_t px = (screen_x + dx) % screen_width;
        canvas.set_pixel(px, y, color);
    }
}

void draw_ground_bumps(Canvas& canvas,
                       int32_t baseline_y,
                       int32_t offset,
                       int32_t screen_width,
                       domain::ColorValue color) {
    const GroundBump* bumps = UiSprites::ground_bumps();
    const size_t count = UiSprites::ground_bump_count();
    for (size_t i = 0; i < count; ++i) {
        const int32_t bx = (bumps[i].x - offset % 600 + 600) % 600;
        const int32_t by = baseline_y + bumps[i].y_offset;
        draw_single_bump(canvas, bx, by, bumps[i].length, screen_width, color);
    }
}

}  // namespace

void SpriteRenderer::draw_dino(Canvas& canvas,
                               int32_t x,
                               int32_t y,
                               domain::PlayerState state,
                               uint8_t leg_step,
                               domain::ColorValue color) {
    const SpriteMask mask = pick_dino_mask(state, leg_step);
    canvas.draw_bitmap(x, y, mask.width, mask.height, mask.data, color);
}

void SpriteRenderer::draw_obstacle(Canvas& canvas,
                                   const domain::Obstacle& obstacle,
                                   uint8_t anim_step,
                                   domain::ColorValue color) {
    const SpriteMask mask = pick_obstacle_mask(obstacle.type(), anim_step);
    const domain::ScreenCoordinate pos = obstacle.position();
    canvas.draw_bitmap(pos.x(), pos.y(), mask.width, mask.height, mask.data, color);
}

void SpriteRenderer::draw_cloud(Canvas& canvas,
                                const domain::Cloud& cloud,
                                domain::ColorValue color) {
    const SpriteMask mask = SpriteData::cloud();
    const domain::ScreenCoordinate pos = cloud.position();
    canvas.draw_bitmap(pos.x(), pos.y(), mask.width, mask.height, mask.data, color);
}

void SpriteRenderer::draw_ground(Canvas& canvas,
                                 const domain::Ground& ground,
                                 int32_t screen_width,
                                 domain::ColorValue color) {
    const int32_t y = ground.baseline_y();
    const domain::BoundingBox ground_line(
        domain::ScreenCoordinate(0, y),
        domain::Dimensions(screen_width, 2));
    canvas.fill_rect(ground_line, color);
    draw_ground_bumps(canvas, y, ground.offset_pixels(), screen_width, color);
}

}  // namespace dino::graphics
