#include "domain/score.h"

namespace dino::domain {

void ScoreBoard::add_points(int32_t delta) {
    current_ = current_.increment(delta);
    if (current_ > high_) {
        high_ = current_;
    }
}

void ScoreBoard::set_high(ScoreValue high) {
    if (high > high_) {
        high_ = high;
    }
}

void ScoreBoard::reset() {
    current_ = ScoreValue(0);
}

int32_t DistanceTracker::advance(FixedPoint speed) {
    accumulated_ = accumulated_.advance(speed);
    remainder_ += speed.to_pixels();
    if (remainder_ < DISTANCE_PER_SCORE_PIXELS) {
        return 0;
    }
    const int32_t points = remainder_ / DISTANCE_PER_SCORE_PIXELS;
    remainder_ %= DISTANCE_PER_SCORE_PIXELS;
    return points;
}

void DistanceTracker::reset() {
    accumulated_ = DistanceValue(FixedPoint::from_pixels(0));
    remainder_ = 0;
}

bool Score::is_at_milestone() const {
    return board_.current().has_reached_milestone(MILESTONE_INTERVAL);
}

void Score::update(FixedPoint speed) {
    const int32_t points = distance_tracker_.advance(speed);
    if (points <= 0) {
        return;
    }
    board_.add_points(points);
}

void Score::set_high_score(ScoreValue high) {
    board_.set_high(high);
}

void Score::reset() {
    board_.reset();
    distance_tracker_.reset();
}

}  // namespace dino::domain
