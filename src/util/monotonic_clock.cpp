#include "util/monotonic_clock.h"

#include <ctime>

namespace dino::util {

Milliseconds MonotonicClock::now_milliseconds() const {
    struct timespec ts {};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    const int64_t seconds = static_cast<int64_t>(ts.tv_sec);
    const int64_t millis = static_cast<int64_t>(ts.tv_nsec) / 1000000LL;
    return Milliseconds(seconds * 1000LL + millis);
}

}  // namespace dino::util
