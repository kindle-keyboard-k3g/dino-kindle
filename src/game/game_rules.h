#ifndef DINO_GAME_GAME_RULES_H_
#define DINO_GAME_GAME_RULES_H_

#include "domain/game_values.h"

namespace dino::game {

/**
 * @brief Core game tuning parameters, physical speed scaling, and limits.
 */
class GameRules {
public:
    static constexpr int32_t SCREEN_WIDTH = 600;
    static constexpr int32_t SCREEN_HEIGHT = 800;
    static constexpr int32_t BASE_SPEED_PIXELS = 8;
    static constexpr int32_t MAX_SPEED_PIXELS = 18;
    static constexpr int32_t SPEED_STEP_SCORE = 100;
    static constexpr int32_t CLOUD_SPEED_PIXELS = 1;
    static constexpr int32_t PTERO_MIN_SCORE = 300;
    static constexpr int32_t MILESTONE_INTERVAL = 700;
    static constexpr int32_t MIN_SPAWN_GAP_PIXELS = 220;
    static constexpr int32_t MAX_SPAWN_GAP_PIXELS = 450;
    static constexpr int32_t TARGET_FRAME_MILLIS = 80;

    [[nodiscard]] static domain::FixedPoint calculate_speed(domain::ScoreValue score);
};

}  // namespace dino::game

#endif  // DINO_GAME_GAME_RULES_H_
