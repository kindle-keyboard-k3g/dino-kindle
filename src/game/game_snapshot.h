#ifndef DINO_GAME_GAME_SNAPSHOT_H_
#define DINO_GAME_GAME_SNAPSHOT_H_

#include "domain/game_world.h"

namespace dino::game {

/**
 * @brief Status flags for rendering overlay and theme.
 */
class RenderStatus {
public:
    constexpr RenderStatus()
        : scores_{}, flags_(0) {}
    constexpr RenderStatus(domain::ScoreBoard scores,
                           bool is_night,
                           bool is_game_over,
                           bool is_milestone)
        : scores_(scores),
          flags_(static_cast<uint8_t>(
              (is_night ? 1 : 0) |
              (is_game_over ? 2 : 0) |
              (is_milestone ? 4 : 0))) {}

    /// Current score points.
    [[nodiscard]] constexpr domain::ScoreValue current_score() const {
        return scores_.current();
    }
    /// Recorded high score points.
    [[nodiscard]] constexpr domain::ScoreValue high_score() const {
        return scores_.high();
    }
    /// True if night-time palette inversion is active.
    [[nodiscard]] constexpr bool is_night() const { return (flags_ & 1) != 0; }
    /// True if game ended due to collision.
    [[nodiscard]] constexpr bool is_game_over() const { return (flags_ & 2) != 0; }
    /// True if score milestone flash effect is active.
    [[nodiscard]] constexpr bool is_milestone() const { return (flags_ & 4) != 0; }

private:
    domain::ScoreBoard scores_;
    uint8_t flags_;
};

/**
 * @brief Immutable snapshot of complete game state for the rendering subsystem.
 */
class GameSnapshot {
public:
    GameSnapshot() = default;
    GameSnapshot(domain::GameWorld world, RenderStatus status)
        : world_(world), status_(status) {}

    /// Game world snapshot containing all actors and environment entities.
    [[nodiscard]] const domain::GameWorld& world() const { return world_; }
    /// Overlay status flags and score data.
    [[nodiscard]] const RenderStatus& status() const { return status_; }

private:
    domain::GameWorld world_;
    RenderStatus status_;
};

}  // namespace dino::game

#endif  // DINO_GAME_GAME_SNAPSHOT_H_
