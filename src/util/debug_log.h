#ifndef DINO_UTIL_DEBUG_LOG_H_
#define DINO_UTIL_DEBUG_LOG_H_

#include <iostream>

namespace dino::util {

/**
 * @brief Zero-overhead compile-time controllable logger.
 */
class DebugLog {
public:
    template <typename... Args>
    static void log([[maybe_unused]] Args&&... args) {
#ifdef DINO_DEBUG_LOG
        (std::cerr << ... << args) << '\n';
#endif
    }
};

}  // namespace dino::util

#endif  // DINO_UTIL_DEBUG_LOG_H_
