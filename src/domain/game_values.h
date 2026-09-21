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

    [[nodiscard]] static constexpr FixedPoint from_pixels(int32_t pixels) {
        return FixedPoint(pixels * ONE_RAW);
    }

    [[nodiscard]] static constexpr FixedPoint from_raw(int32_t raw) {
        return FixedPoint(raw);
    }

    [[nodiscard]] constexpr int32_t to_pixels() const {
        return raw_value_ >> FRACTIONAL_BITS;
    }

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

    [[nodiscard]] constexpr int32_t points() const { return points_; }

    [[nodiscard]] constexpr bool has_reached_milestone(int32_t interval) const {
        if (points_ <= 0 || interval <= 0) {
            return false;
        }
        return (points_ % interval) == 0;
    }

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

    [[nodiscard]] constexpr FixedPoint value() const { return fixed_distance_; }

    [[nodiscard]] constexpr DistanceValue advance(FixedPoint delta) const {
        return DistanceValue(fixed_distance_ + delta);
    }

private:
    FixedPoint fixed_distance_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_GAME_VALUES_H_
