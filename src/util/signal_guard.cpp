#include "util/signal_guard.h"

namespace dino::util {

volatile sig_atomic_t SignalGuard::interrupted_flag_ = 0;

void SignalGuard::handle_signal([[maybe_unused]] int signal_number) {
    interrupted_flag_ = 1;
}

SignalGuard::SignalGuard() {
    interrupted_flag_ = 0;
    std::signal(SIGINT, &SignalGuard::handle_signal);
    std::signal(SIGTERM, &SignalGuard::handle_signal);
}

SignalGuard::~SignalGuard() {
    std::signal(SIGINT, SIG_DFL);
    std::signal(SIGTERM, SIG_DFL);
}

bool SignalGuard::is_interrupted() {
    return interrupted_flag_ != 0;
}

void SignalGuard::trigger_shutdown() {
    interrupted_flag_ = 1;
}

}  // namespace dino::util
