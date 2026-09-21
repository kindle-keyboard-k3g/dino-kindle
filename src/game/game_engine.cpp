#include "game/game_engine.h"
#include "game/collision_detector.h"
#include "game/game_rules.h"

namespace dino::game {

GameEngine::GameEngine(uint32_t seed)
    : entities_(seed), state_() {}

void GameEngine::apply_input(const InputState& input) {
    auto& player = entities_.world().actors().player();
    if (input.is_jump_requested()) {
        player.jump();
    }
    if (input.is_ducking()) {
        player.duck();
        return;
    }
    player.stand();
}

void GameEngine::update_movement(domain::FixedPoint speed) {
    auto& actors = entities_.world().actors();
    auto& env = entities_.world().environment();
    actors.player().update();
    actors.obstacles().update_all(speed);
    actors.obstacles().prune_off_screen();
    const auto cloud_speed = domain::FixedPoint::from_pixels(GameRules::CLOUD_SPEED_PIXELS);
    env.clouds().update_all(cloud_speed);
    env.clouds().prune_off_screen();
    env.ground().update(speed);
}

void GameEngine::check_collisions() {
    auto& actors = entities_.world().actors();
    if (!CollisionDetector::has_collision(actors.player(), actors.obstacles())) {
        return;
    }
    actors.player().crash();
    state_.set_game_over(true);
}

void GameEngine::check_milestone(int32_t old_points) {
    auto& status = state_.status();
    const int32_t current_points = status.score().current().points();
    const int32_t old_milestone = old_points / GameRules::MILESTONE_INTERVAL;
    const int32_t new_milestone = current_points / GameRules::MILESTONE_INTERVAL;
    if (new_milestone > old_milestone && current_points > 0) {
        status.theme().toggle_mode();
        status.theme().set_milestone_active(true);
        return;
    }
    status.theme().set_milestone_active(false);
}

void GameEngine::step(const InputState& input) {
    if (state_.is_game_over()) {
        if (input.is_restart_requested()) {
            restart();
        }
        return;
    }
    apply_input(input);
    const domain::FixedPoint speed =
        GameRules::calculate_speed(state_.status().score().current());
    update_movement(speed);
    check_collisions();
    if (state_.is_game_over()) {
        return;
    }
    auto& actors = entities_.world().actors();
    auto& env = entities_.world().environment();
    entities_.spawner().update(
        actors.obstacles(), env.clouds(), state_.status().score().current());
    const int32_t old_points = state_.status().score().current().points();
    state_.status().score().update(speed);
    check_milestone(old_points);
}

void GameEngine::restart() {
    entities_.reset();
    state_.reset();
}

void GameEngine::set_high_score(domain::ScoreValue high) {
    state_.status().score().set_high_score(high);
}

void GameEngine::inject_obstacle_for_testing(const domain::Obstacle& obstacle) {
    entities_.world().actors().obstacles().add(obstacle);
}

GameSnapshot GameEngine::snapshot() const {
    domain::ScoreBoard board;
    board.add_points(state_.status().score().current().points());
    board.set_high(state_.status().score().high());
    const RenderStatus status(
        board,
        state_.status().theme().is_night(),
        state_.is_game_over(),
        state_.status().theme().is_milestone_active());
    return GameSnapshot(entities_.world(), status);
}

}  // namespace dino::game
