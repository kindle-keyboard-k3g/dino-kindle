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

    /**
     * @brief Returns current active game score.
     * @return Current ScoreValue.
     */
    [[nodiscard]] constexpr ScoreValue current() const { return current_; }

    /**
     * @brief Returns recorded high score.
     * @return High ScoreValue.
     */
    [[nodiscard]] constexpr ScoreValue high() const { return high_; }

    /**
     * @brief Adds points to current score and updates high score if beaten.
     * @param delta Number of points to add.
     */
    void add_points(int32_t delta);

    /**
     * @brief Sets the recorded high score.
     * @param high Loaded high score value.
     */
    void set_high(ScoreValue high);

    /**
     * @brief Resets current score to zero while preserving high score.
     */
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

    /**
     * @brief Advances distance by current speed and calculates points earned.
     * @param speed Current world scroll velocity.
     * @return Discrete points scored this step.
     */
    [[nodiscard]] int32_t advance(FixedPoint speed);

    /**
     * @brief Resets accumulated distance to zero.
     */
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

    /**
     * @brief Returns current active game score.
     * @return Current ScoreValue.
     */
    [[nodiscard]] ScoreValue current() const { return board_.current(); }

    /**
     * @brief Returns recorded high score.
     * @return High ScoreValue.
     */
    [[nodiscard]] ScoreValue high() const { return board_.high(); }

    /**
     * @brief Checks if current score sits on a milestone boundary.
     * @return True if milestone reached.
     */
    [[nodiscard]] bool is_at_milestone() const;

    /**
     * @brief Updates distance and score board using current speed.
     * @param speed World scroll velocity.
     */
    void update(FixedPoint speed);

    /**
     * @brief Overrides or sets high score on board.
     * @param high New high score.
     */
    void set_high_score(ScoreValue high);

    /**
     * @brief Resets current score and distance tracker.
     */
    void reset();

private:
    ScoreBoard board_;
    DistanceTracker distance_tracker_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_SCORE_H_
