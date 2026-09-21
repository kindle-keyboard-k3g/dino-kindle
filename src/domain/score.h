#ifndef DINO_DOMAIN_SCORE_H_
#define DINO_DOMAIN_SCORE_H_

#include "domain/game_values.h"

namespace dino::domain {

/**
 * @brief Holds current and high score values.
 */
class ScoreBoard {
public:
    constexpr ScoreBoard()
        : current_(ScoreValue(0)), high_(ScoreValue(0)) {}

    [[nodiscard]] constexpr ScoreValue current() const { return current_; }
    [[nodiscard]] constexpr ScoreValue high() const { return high_; }

    void add_points(int32_t delta);
    void set_high(ScoreValue high);
    void reset();

private:
    ScoreValue current_;
    ScoreValue high_;
};

/**
 * @brief Converts continuous distance into discrete score increments.
 */
class DistanceTracker {
public:
    static constexpr int32_t DISTANCE_PER_SCORE_PIXELS = 10;

    constexpr DistanceTracker()
        : accumulated_(DistanceValue(FixedPoint::from_pixels(0))),
          remainder_(0) {}

    [[nodiscard]] int32_t advance(FixedPoint speed);
    void reset();

private:
    DistanceValue accumulated_;
    int32_t remainder_;
};

/**
 * @brief Complete score aggregate tracking distance and milestone triggers.
 */
class Score {
public:
    static constexpr int32_t MILESTONE_INTERVAL = 700;

    Score() = default;

    [[nodiscard]] ScoreValue current() const { return board_.current(); }
    [[nodiscard]] ScoreValue high() const { return board_.high(); }
    [[nodiscard]] bool is_at_milestone() const;

    void update(FixedPoint speed);
    void set_high_score(ScoreValue high);
    void reset();

private:
    ScoreBoard board_;
    DistanceTracker distance_tracker_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_SCORE_H_
