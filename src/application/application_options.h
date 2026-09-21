#ifndef DINO_APPLICATION_APPLICATION_OPTIONS_H_
#define DINO_APPLICATION_APPLICATION_OPTIONS_H_

#include <cstdint>

namespace dino::application {

/**
 * @brief Terminal, PPM, or hardware display selection flags.
 */
class ExecutionMode {
public:
    constexpr ExecutionMode()
        : ansi_mode_(false), ppm_mode_(false) {}

    [[nodiscard]] constexpr bool is_ansi() const { return ansi_mode_; }
    [[nodiscard]] constexpr bool is_ppm() const { return ppm_mode_; }

    void set_ansi(bool enabled) { ansi_mode_ = enabled; }
    void set_ppm(bool enabled) { ppm_mode_ = enabled; }

private:
    bool ansi_mode_;
    bool ppm_mode_;
};

/**
 * @brief Frame count caps and PRNG seeds.
 */
class ExecutionLimits {
public:
    constexpr ExecutionLimits()
        : seed_(0x12345678U), max_frames_(-1) {}

    [[nodiscard]] constexpr uint32_t seed() const { return seed_; }
    [[nodiscard]] constexpr int32_t max_frames() const { return max_frames_; }

    void set_seed(uint32_t s) { seed_ = s; }
    void set_max_frames(int32_t f) { max_frames_ = f; }

private:
    uint32_t seed_;
    int32_t max_frames_;
};

/**
 * @brief Parsed command line parameters.
 */
class ApplicationOptions {
public:
    ApplicationOptions() = default;

    [[nodiscard]] static ApplicationOptions parse(int argc, char* argv[]);

    [[nodiscard]] bool is_ansi() const { return mode_.is_ansi(); }
    [[nodiscard]] bool is_ppm() const { return mode_.is_ppm(); }
    [[nodiscard]] uint32_t seed() const { return limits_.seed(); }
    [[nodiscard]] int32_t max_frames() const { return limits_.max_frames(); }

    void set_ansi(bool val) { mode_.set_ansi(val); }
    void set_ppm(bool val) { mode_.set_ppm(val); }
    void set_seed(uint32_t val) { limits_.set_seed(val); }
    void set_max_frames(int32_t val) { limits_.set_max_frames(val); }

private:
    ExecutionMode mode_;
    ExecutionLimits limits_;
};

}  // namespace dino::application

#endif  // DINO_APPLICATION_APPLICATION_OPTIONS_H_
