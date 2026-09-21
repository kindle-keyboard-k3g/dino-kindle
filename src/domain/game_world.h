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

    [[nodiscard]] Player& player() { return player_; }
    [[nodiscard]] const Player& player() const { return player_; }
    [[nodiscard]] ObstacleCollection& obstacles() { return obstacles_; }
    [[nodiscard]] const ObstacleCollection& obstacles() const {
        return obstacles_;
    }

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

    [[nodiscard]] CloudCollection& clouds() { return clouds_; }
    [[nodiscard]] const CloudCollection& clouds() const { return clouds_; }
    [[nodiscard]] Ground& ground() { return ground_; }
    [[nodiscard]] const Ground& ground() const { return ground_; }

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

    [[nodiscard]] Score& score() { return score_; }
    [[nodiscard]] const Score& score() const { return score_; }
    [[nodiscard]] Theme& theme() { return theme_; }
    [[nodiscard]] const Theme& theme() const { return theme_; }

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

    [[nodiscard]] ActorGroup& actors() { return actors_; }
    [[nodiscard]] const ActorGroup& actors() const { return actors_; }
    [[nodiscard]] EnvironmentGroup& environment() { return environment_; }
    [[nodiscard]] const EnvironmentGroup& environment() const {
        return environment_;
    }

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
