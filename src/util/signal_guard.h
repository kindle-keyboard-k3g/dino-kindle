#ifndef DINO_UTIL_SIGNAL_GUARD_H_
#define DINO_UTIL_SIGNAL_GUARD_H_

#include <csignal>

namespace dino::util {

/**
 * @brief Traps termination signals and manages graceful shutdown flag.
 */
class SignalGuard {
public:
    SignalGuard();
    ~SignalGuard();

    SignalGuard(const SignalGuard&) = delete;
    SignalGuard& operator=(const SignalGuard&) = delete;

    [[nodiscard]] static bool is_interrupted();
    static void trigger_shutdown();

private:
    static void handle_signal(int signal_number);
    static volatile sig_atomic_t interrupted_flag_;
};

}  // namespace dino::util

#endif  // DINO_UTIL_SIGNAL_GUARD_H_
