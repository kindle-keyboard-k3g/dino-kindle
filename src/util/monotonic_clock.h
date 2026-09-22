#ifndef DINO_UTIL_MONOTONIC_CLOCK_H_
#define DINO_UTIL_MONOTONIC_CLOCK_H_

#include <cstdint>

namespace dino::util {

/**
 * @brief Strongly-typed millisecond duration.
 */
class Milliseconds {
public:
    /**
     * @brief Constructs a duration with raw millisecond count.
     * @param count Total elapsed milliseconds.
     */
    explicit constexpr Milliseconds(int64_t count) : count_(count) {}

    /**
     * @brief Returns the inner millisecond count.
     * @return Integer duration in milliseconds.
     */
    [[nodiscard]] constexpr int64_t count() const { return count_; }

    constexpr bool operator<(const Milliseconds& other) const {
        return count_ < other.count_;
    }

    constexpr bool operator>=(const Milliseconds& other) const {
        return count_ >= other.count_;
    }

private:
    int64_t count_;
};

/**
 * @brief Monotonic clock returning elapsed milliseconds.
 */
class MonotonicClock {
public:
    virtual ~MonotonicClock() = default;

    /**
     * @brief Reads current monotonic time in milliseconds.
     * @return Current timestamp as Milliseconds.
     */
    [[nodiscard]] virtual Milliseconds now_milliseconds() const;
};

}  // namespace dino::util

#endif  // DINO_UTIL_MONOTONIC_CLOCK_H_
