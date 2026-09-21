#include "test_framework.h"
#include "domain/score.h"
#include "domain/theme.h"
#include "game/game_rules.h"

using namespace dino;

DINO_TEST_CASE(TestScoreDistanceAccumulation) {
    domain::Score score;
    ASSERT_EQ(score.current().points(), 0);

    // Advance 100 pixels (10 points at 10px per point)
    score.update(domain::FixedPoint::from_pixels(100));
    ASSERT_EQ(score.current().points(), 10);
}

DINO_TEST_CASE(TestScoreMilestone) {
    domain::ScoreValue s0(0);
    domain::ScoreValue s699(699);
    domain::ScoreValue s700(700);

    ASSERT_FALSE(s0.has_reached_milestone(700));
    ASSERT_FALSE(s699.has_reached_milestone(700));
    ASSERT_TRUE(s700.has_reached_milestone(700));
}

DINO_TEST_CASE(TestThemeDayNightColors) {
    domain::Theme theme;
    ASSERT_FALSE(theme.is_night());
    ASSERT_EQ(theme.background_color(), domain::ColorValue::White);
    ASSERT_EQ(theme.foreground_color(), domain::ColorValue::Black);

    theme.toggle_mode();
    ASSERT_TRUE(theme.is_night());
    ASSERT_EQ(theme.background_color(), domain::ColorValue::Black);
    ASSERT_EQ(theme.foreground_color(), domain::ColorValue::White);
}

DINO_TEST_CASE(TestSpeedAcceleration) {
    const auto speed0 = game::GameRules::calculate_speed(domain::ScoreValue(0));
    const auto speed100 = game::GameRules::calculate_speed(domain::ScoreValue(100));
    const auto speed2000 = game::GameRules::calculate_speed(domain::ScoreValue(2000));

    ASSERT_EQ(speed0.to_pixels(), game::GameRules::BASE_SPEED_PIXELS);
    ASSERT_TRUE(speed100 > speed0);
    ASSERT_EQ(speed2000.to_pixels(), game::GameRules::MAX_SPEED_PIXELS);
}
