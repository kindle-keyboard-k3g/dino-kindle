#include "test_framework.h"
#include "game/game_engine.h"
#include "graphics/scene_renderer.h"
#include "graphics/sprite_data.h"
#include "graphics/ui_sprites.h"

using namespace dino::graphics;

DINO_TEST_CASE(TestDinoSpriteDimensions) {
    const SpriteMask stand = SpriteData::dino_standing();
    ASSERT_EQ(stand.width, 44);
    ASSERT_EQ(stand.height, 48);
    ASSERT_TRUE(stand.data != nullptr);

    const SpriteMask run_l = SpriteData::dino_run_left();
    ASSERT_EQ(run_l.width, 44);
    ASSERT_EQ(run_l.height, 48);

    const SpriteMask run_r = SpriteData::dino_run_right();
    ASSERT_EQ(run_r.width, 44);
    ASSERT_EQ(run_r.height, 48);

    const SpriteMask duck_l = SpriteData::dino_duck_left();
    ASSERT_EQ(duck_l.width, 56);
    ASSERT_EQ(duck_l.height, 28);

    const SpriteMask duck_r = SpriteData::dino_duck_right();
    ASSERT_EQ(duck_r.width, 56);
    ASSERT_EQ(duck_r.height, 28);

    const SpriteMask crashed = SpriteData::dino_crashed();
    ASSERT_EQ(crashed.width, 44);
    ASSERT_EQ(crashed.height, 48);
}

DINO_TEST_CASE(TestObstacleSpriteDimensions) {
    const SpriteMask small_c = SpriteData::small_cactus();
    ASSERT_EQ(small_c.width, 20);
    ASSERT_EQ(small_c.height, 36);
    ASSERT_TRUE(small_c.data != nullptr);

    const SpriteMask large_c = SpriteData::large_cactus();
    ASSERT_EQ(large_c.width, 32);
    ASSERT_EQ(large_c.height, 48);

    const SpriteMask ptero_up = SpriteData::ptero_wing_up();
    ASSERT_EQ(ptero_up.width, 42);
    ASSERT_EQ(ptero_up.height, 32);

    const SpriteMask ptero_down = SpriteData::ptero_wing_down();
    ASSERT_EQ(ptero_down.width, 42);
    ASSERT_EQ(ptero_down.height, 32);

    const SpriteMask cloud = SpriteData::cloud();
    ASSERT_EQ(cloud.width, 46);
    ASSERT_EQ(cloud.height, 14);
}

DINO_TEST_CASE(TestUiSpriteDimensions) {
    const SpriteMask game_over = UiSprites::game_over_banner();
    ASSERT_EQ(game_over.width, 191);
    ASSERT_EQ(game_over.height, 11);
    ASSERT_TRUE(game_over.data != nullptr);

    const SpriteMask restart = UiSprites::restart_button();
    ASSERT_EQ(restart.width, 36);
    ASSERT_EQ(restart.height, 32);

    const SpriteMask score_hi = UiSprites::score_hi();
    ASSERT_EQ(score_hi.width, 20);
    ASSERT_EQ(score_hi.height, 13);

    for (int32_t d = 0; d <= 9; ++d) {
        const SpriteMask digit = UiSprites::score_digit(d);
        ASSERT_EQ(digit.width, 10);
        ASSERT_EQ(digit.height, 13);
        ASSERT_TRUE(digit.data != nullptr);
    }

    ASSERT_EQ(UiSprites::ground_bump_count(), 44);
    ASSERT_TRUE(UiSprites::ground_bumps() != nullptr);
}

DINO_TEST_CASE(TestSceneRendererGameOverRendering) {
    dino::game::GameEngine engine(0x1234U);
    dino::game::InputState input;
    engine.inject_obstacle_for_testing(dino::domain::Obstacle(
        dino::domain::ObstacleType::SmallCactus,
        dino::domain::FixedPoint::from_pixels(dino::domain::Player::X_POSITION)));
    engine.step(input);
    ASSERT_TRUE(engine.is_game_over());

    dino::graphics::Canvas canvas(dino::domain::Dimensions(600, 800));
    dino::graphics::SceneRenderer renderer;
    renderer.render(canvas, engine.snapshot());

    // Game Over banner is drawn at Y=240, centered horizontally
    const int32_t banner_x = (600 - 191) / 2;
    // Check that at least some pixels in the banner area are Black
    int black_pixels_banner = 0;
    for (int y = 240; y < 240 + 11; ++y) {
        for (int x = banner_x; x < banner_x + 191; ++x) {
            if (canvas.get_pixel(x, y) == dino::domain::ColorValue::Black) {
                ++black_pixels_banner;
            }
        }
    }
    ASSERT_TRUE(black_pixels_banner > 100);

    // Restart button is drawn at Y=270, centered horizontally
    const int32_t restart_x = (600 - 36) / 2;
    int black_pixels_restart = 0;
    for (int y = 270; y < 270 + 32; ++y) {
        for (int x = restart_x; x < restart_x + 36; ++x) {
            if (canvas.get_pixel(x, y) == dino::domain::ColorValue::Black) {
                ++black_pixels_restart;
            }
        }
    }
    ASSERT_TRUE(black_pixels_restart > 50);
}

