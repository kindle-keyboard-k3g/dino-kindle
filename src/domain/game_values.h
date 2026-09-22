#ifndef DINO_DOMAIN_GAME_VALUES_H_
#define DINO_DOMAIN_GAME_VALUES_H_

#include <cstdint>

namespace dino::domain {

/**
 * @brief 16.16 Fixed-point arithmetic value for deterministic, float-free physics.
 */
class FixedPoint {
public:
    static constexpr int32_t FRACTIONAL_BITS = 16;
    static constexpr int32_t ONE_RAW = 1 << FRACTIONAL_BITS;

    constexpr FixedPoint() : raw_value_(0) {}
    explicit constexpr FixedPoint(int32_t raw_value) : raw_value_(raw_value) {}

    /**
     * @brief Creates FixedPoint from integer pixel value.
     * @param pixels Integer pixel count.
     * @return Equivalent FixedPoint.
     */
    [[nodiscard]] static constexpr FixedPoint from_pixels(int32_t pixels) {
        return FixedPoint(pixels * ONE_RAW);
    }

    /**
     * @brief Wraps raw 16.16 representation directly.
     * @param raw Raw 32-bit fixed point value.
     * @return Equivalent FixedPoint.
     */
    [[nodiscard]] static constexpr FixedPoint from_raw(int32_t raw) {
        return FixedPoint(raw);
    }

    /**
     * @brief Converts fixed-point value to whole pixels (truncating fraction).
     * @return Truncated pixel value.
     */
    [[nodiscard]] constexpr int32_t to_pixels() const {
        return raw_value_ >> FRACTIONAL_BITS;
    }

    /**
     * @brief Returns underlying raw integer value.
     * @return 16.16 raw integer.
     */
    [[nodiscard]] constexpr int32_t raw() const { return raw_value_; }

    constexpr FixedPoint operator+(const FixedPoint& other) const {
        return FixedPoint(raw_value_ + other.raw_value_);
    }

    constexpr FixedPoint operator-(const FixedPoint& other) const {
        return FixedPoint(raw_value_ - other.raw_value_);
    }

    constexpr bool operator<(const FixedPoint& other) const {
        return raw_value_ < other.raw_value_;
    }

    constexpr bool operator>(const FixedPoint& other) const {
        return raw_value_ > other.raw_value_;
    }

    constexpr bool operator<=(const FixedPoint& other) const {
        return raw_value_ <= other.raw_value_;
    }

    constexpr bool operator>=(const FixedPoint& other) const {
        return raw_value_ >= other.raw_value_;
    }

    constexpr bool operator==(const FixedPoint& other) const {
        return raw_value_ == other.raw_value_;
    }

private:
    int32_t raw_value_;
};

/**
 * @brief Represents player or obstacle velocity.
 */
class Velocity {
public:
    constexpr Velocity() : horizontal_{}, vertical_{} {}
    constexpr Velocity(FixedPoint horizontal, FixedPoint vertical)
        : horizontal_(horizontal), vertical_(vertical) {}

    [[nodiscard]] constexpr FixedPoint horizontal() const { return horizontal_; }
    [[nodiscard]] constexpr FixedPoint vertical() const { return vertical_; }

private:
    FixedPoint horizontal_;
    FixedPoint vertical_;
};

/**
 * @brief Strongly-typed score representation with milestone detection.
 */
class ScoreValue {
public:
    explicit constexpr ScoreValue(int32_t points) : points_(points) {}

    /**
     * @brief Returns numerical point count.
     * @return Score points.
     */
    [[nodiscard]] constexpr int32_t points() const { return points_; }

    /**
     * @brief Determines if score reached a multiple of given interval.
     * @param interval Interval stride (e.g. 700).
     * @return True if milestone is hit.
     */
    [[nodiscard]] constexpr bool has_reached_milestone(int32_t interval) const {
        if (points_ <= 0 || interval <= 0) {
            return false;
        }
        return (points_ % interval) == 0;
    }

    /**
     * @brief Produces a new ScoreValue incremented by delta points.
     * @param delta Points to add.
     * @return Incremented ScoreValue.
     */
    [[nodiscard]] constexpr ScoreValue increment(int32_t delta) const {
        return ScoreValue(points_ + delta);
    }

    constexpr bool operator>(const ScoreValue& other) const {
        return points_ > other.points_;
    }

    constexpr bool operator==(const ScoreValue& other) const {
        return points_ == other.points_;
    }

private:
    int32_t points_;
};

/**
 * @brief Accumulated distance value in fixed point.
 */
class DistanceValue {
public:
    constexpr DistanceValue() : fixed_distance_{} {}
    explicit constexpr DistanceValue(FixedPoint distance)
        : fixed_distance_(distance) {}

    /**
     * @brief Returns fixed point distance value.
     * @return Inner FixedPoint distance.
     */
    [[nodiscard]] constexpr FixedPoint value() const { return fixed_distance_; }

    /**
     * @brief Produces an advanced distance value by delta.
     * @param delta Distance to add.
     * @return Advanced DistanceValue.
     */
    [[nodiscard]] constexpr DistanceValue advance(FixedPoint delta) const {
        return DistanceValue(fixed_distance_ + delta);
    }

private:
    FixedPoint fixed_distance_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_GAME_VALUES_H_
