#ifndef DINO_DOMAIN_GAME_WORLD_H_
#define DINO_DOMAIN_GAME_WORLD_H_

#include "domain/cloud_collection.h"
#include "domain/ground.h"
#include "domain/obstacle_collection.h"
#include "domain/player.h"
#include "domain/score.h"
#include "domain/theme.h"

namespace dino::domain {

/**
 * @brief Dynamic actors capable of colliding.
 */
class ActorGroup {
public:
    ActorGroup() = default;

    /// Mutable reference to player dino.
    [[nodiscard]] Player& player() { return player_; }
    /// Const reference to player dino.
    [[nodiscard]] const Player& player() const { return player_; }
    /// Mutable reference to active obstacles collection.
    [[nodiscard]] ObstacleCollection& obstacles() { return obstacles_; }
    /// Const reference to active obstacles collection.
    [[nodiscard]] const ObstacleCollection& obstacles() const {
        return obstacles_;
    }

    /// Resets player and clears all active obstacles.
    void reset() {
        player_.reset();
        obstacles_.clear();
    }

private:
    Player player_;
    ObstacleCollection obstacles_;
};

/**
 * @brief Decorative and terrain environmental entities.
 */
class EnvironmentGroup {
public:
    EnvironmentGroup() = default;

    /// Mutable reference to decorative clouds.
    [[nodiscard]] CloudCollection& clouds() { return clouds_; }
    /// Const reference to decorative clouds.
    [[nodiscard]] const CloudCollection& clouds() const { return clouds_; }
    /// Mutable reference to scrolling ground terrain.
    [[nodiscard]] Ground& ground() { return ground_; }
    /// Const reference to scrolling ground terrain.
    [[nodiscard]] const Ground& ground() const { return ground_; }

    /// Clears clouds and resets ground scroll offset.
    void reset() {
        clouds_.clear();
        ground_.reset();
    }

private:
    CloudCollection clouds_;
    Ground ground_;
};

/**
 * @brief Score and visual day/night theme state.
 */
class WorldStatus {
public:
    WorldStatus() = default;

    /// Mutable reference to score tracking.
    [[nodiscard]] Score& score() { return score_; }
    /// Const reference to score tracking.
    [[nodiscard]] const Score& score() const { return score_; }
    /// Mutable reference to theme and palette state.
    [[nodiscard]] Theme& theme() { return theme_; }
    /// Const reference to theme and palette state.
    [[nodiscard]] const Theme& theme() const { return theme_; }

    /// Resets score state.
    void reset() {
        score_.reset();
    }

private:
    Score score_;
    Theme theme_;
};

/**
 * @brief Aggregate container for all game world entities and status.
 */
class GameWorld {
public:
    GameWorld() = default;

    /// Mutable reference to actor entities (player and obstacles).
    [[nodiscard]] ActorGroup& actors() { return actors_; }
    /// Const reference to actor entities.
    [[nodiscard]] const ActorGroup& actors() const { return actors_; }
    /// Mutable reference to environmental entities (clouds and ground).
    [[nodiscard]] EnvironmentGroup& environment() { return environment_; }
    /// Const reference to environmental entities.
    [[nodiscard]] const EnvironmentGroup& environment() const {
        return environment_;
    }

    /// Resets both actors and environment to initial state.
    void reset() {
        actors_.reset();
        environment_.reset();
    }

private:
    ActorGroup actors_;
    EnvironmentGroup environment_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_GAME_WORLD_H_
