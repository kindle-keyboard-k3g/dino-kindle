#ifndef DINO_GAME_GAME_ENGINE_H_
#define DINO_GAME_GAME_ENGINE_H_

#include "domain/game_world.h"
#include "game/game_snapshot.h"
#include "game/input_state.h"
#include "game/obstacle_spawner.h"

namespace dino::game {

/**
 * @brief Dynamic entity collections and generator for the engine.
 */
class EngineEntities {
public:
    explicit EngineEntities(uint32_t seed = 0x87654321U)
        : spawner_(seed) {}

    [[nodiscard]] domain::GameWorld& world() { return world_; }
    [[nodiscard]] const domain::GameWorld& world() const { return world_; }
    [[nodiscard]] ObstacleSpawner& spawner() { return spawner_; }

    void reset() {
        world_.reset();
        spawner_.reset();
    }

private:
    domain::GameWorld world_;
    ObstacleSpawner spawner_;
};

/**
 * @brief World state and game-over status flags.
 */
class EngineState {
public:
    EngineState() : is_game_over_(false) {}

    [[nodiscard]] domain::WorldStatus& status() { return status_; }
    [[nodiscard]] const domain::WorldStatus& status() const { return status_; }
    [[nodiscard]] bool is_game_over() const { return is_game_over_; }

    void set_game_over(bool over) { is_game_over_ = over; }
    void reset() {
        status_.reset();
        is_game_over_ = false;
    }

private:
    domain::WorldStatus status_;
    bool is_game_over_;
};

/**
 * @brief Step coordinator: input -> physics -> collision -> state update.
 */
class GameEngine {
public:
    explicit GameEngine(uint32_t seed = 0x87654321U);

    void step(const InputState& input);
    void restart();
    void set_high_score(domain::ScoreValue high);

    [[nodiscard]] GameSnapshot snapshot() const;
    [[nodiscard]] bool is_game_over() const { return state_.is_game_over(); }

    void inject_obstacle_for_testing(const domain::Obstacle& obstacle);

private:
    void apply_input(const InputState& input);
    void update_movement(domain::FixedPoint speed);
    void check_collisions();
    void check_milestone(int32_t old_points);

    EngineEntities entities_;
    EngineState state_;
};

}  // namespace dino::game

#endif  // DINO_GAME_GAME_ENGINE_H_
