#ifndef DINO_UTIL_DEBUG_LOG_H_
#define DINO_UTIL_DEBUG_LOG_H_

#include <iostream>

namespace dino::util {

/**
 * @brief Zero-overhead compile-time and runtime controllable logger.
 */
class DebugLog {
public:
    /// Enables or disables runtime diagnostic logging.
    static void set_enabled(bool enabled) {
        is_enabled_ = enabled;
    }

    /// Checks if runtime logging is active.
    [[nodiscard]] static bool is_enabled() {
#ifdef DINO_DEBUG_LOG
        return true;
#else
        return is_enabled_;
#endif
    }

    /**
     * @brief Formats and writes debug messages to stderr when enabled.
     * @tparam Args Variadic argument types streamable to std::cerr.
     * @param args Values or text strings to print.
     */
    template <typename... Args>
    static void log([[maybe_unused]] Args&&... args) {
        if (!is_enabled()) {
            return;
        }
        (std::cerr << ... << args) << '\n';
    }

private:
    inline static bool is_enabled_ = false;
};

}  // namespace dino::util

#endif  // DINO_UTIL_DEBUG_LOG_H_
