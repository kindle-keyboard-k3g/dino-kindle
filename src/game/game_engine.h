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
    EngineState() : is_game_over_(false), is_godmode_(false) {}

    [[nodiscard]] domain::WorldStatus& status() { return status_; }
    [[nodiscard]] const domain::WorldStatus& status() const { return status_; }
    [[nodiscard]] bool is_game_over() const { return is_game_over_; }
    [[nodiscard]] bool is_godmode() const { return is_godmode_; }

    void set_game_over(bool over) { is_game_over_ = over; }
    void set_godmode(bool godmode) { is_godmode_ = godmode; }
    void reset() {
        status_.reset();
        is_game_over_ = false;
    }

private:
    domain::WorldStatus status_;
    bool is_game_over_;
    bool is_godmode_;
};

/**
 * @brief Step coordinator: input -> physics -> collision -> state update.
 */
class GameEngine {
public:
    /**
     * @brief Initializes engine entities, spawner, and initial state.
     * @param seed Random seed for obstacle generation.
     */
    explicit GameEngine(uint32_t seed = 0x87654321U);

    /**
     * @brief Advances simulation by one discrete frame tick.
     * @param input Active input state (jumps, ducks, restarts).
     */
    void step(const InputState& input);

    /**
     * @brief Resets game world for a new run while retaining high score.
     */
    void restart();

    /**
     * @brief Injects loaded high score from persistent storage.
     * @param high Loaded high score value.
     */
    void set_high_score(domain::ScoreValue high);

    /**
     * @brief Captures an immutable snapshot of current game state.
     * @return GameSnapshot for renderer.
     */
    [[nodiscard]] GameSnapshot snapshot() const;

    /**
     * @brief Checks if current game is in game-over state.
     * @return True if game over.
     */
    [[nodiscard]] bool is_game_over() const { return state_.is_game_over(); }

    /// Sets godmode to prevent game-over on obstacle collision.
    void set_godmode(bool godmode) { state_.set_godmode(godmode); }

    /**
     * @brief Injects an obstacle for deterministic collision unit testing.
     * @param obstacle Test obstacle instance.
     */
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
