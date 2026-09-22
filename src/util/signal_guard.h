#ifndef DINO_UTIL_SIGNAL_GUARD_H_
#define DINO_UTIL_SIGNAL_GUARD_H_

#include <csignal>

namespace dino::util {

/**
 * @brief Traps termination signals and manages graceful shutdown flag.
 */
class SignalGuard {
public:
    /**
     * @brief Installs POSIX signal handlers for SIGINT and SIGTERM.
     */
    SignalGuard();

    /**
     * @brief Restores default signal actions.
     */
    ~SignalGuard();

    SignalGuard(const SignalGuard&) = delete;
    SignalGuard& operator=(const SignalGuard&) = delete;

    /**
     * @brief Checks whether a termination signal was caught.
     * @return True if shutdown has been requested.
     */
    [[nodiscard]] static bool is_interrupted();

    /**
     * @brief Programmatically flags application shutdown.
     */
    static void trigger_shutdown();

private:
    static void handle_signal(int signal_number);
    static volatile sig_atomic_t interrupted_flag_;
};

}  // namespace dino::util

#endif  // DINO_UTIL_SIGNAL_GUARD_H_
