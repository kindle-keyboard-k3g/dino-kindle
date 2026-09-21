#include "game/game_rules.h"

#include <algorithm>

namespace dino::game {

domain::FixedPoint GameRules::calculate_speed(domain::ScoreValue score) {
    const int32_t bonus = score.points() / SPEED_STEP_SCORE;
    const int32_t speed_pixels = std::min(BASE_SPEED_PIXELS + bonus, MAX_SPEED_PIXELS);
    return domain::FixedPoint::from_pixels(speed_pixels);
}

}  // namespace dino::game
