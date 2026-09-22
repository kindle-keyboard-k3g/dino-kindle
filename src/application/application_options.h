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
 * @brief Diagnostic and gameplay assistance debug options.
 */
class DebugOptions {
public:
    constexpr DebugOptions()
        : debug_mode_(false), godmode_(false) {}

    [[nodiscard]] constexpr bool is_debug() const { return debug_mode_; }
    [[nodiscard]] constexpr bool is_godmode() const { return godmode_; }

    void set_debug(bool enabled) { debug_mode_ = enabled; }
    void set_godmode(bool enabled) { godmode_ = enabled; }

private:
    bool debug_mode_;
    bool godmode_;
};

/**
 * @brief Bundles display modes and debugging options.
 */
class ApplicationFlags {
public:
    ApplicationFlags() = default;

    [[nodiscard]] const ExecutionMode& mode() const { return mode_; }
    [[nodiscard]] ExecutionMode& mode() { return mode_; }
    [[nodiscard]] const DebugOptions& debug() const { return debug_; }
    [[nodiscard]] DebugOptions& debug() { return debug_; }

private:
    ExecutionMode mode_;
    DebugOptions debug_;
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

    /**
     * @brief Parses command-line arguments (--ansi, --ppm, --seed, --frames).
     * @param argc Argument count.
     * @param argv Argument vector.
     * @return Populated ApplicationOptions.
     */
    [[nodiscard]] static ApplicationOptions parse(int argc, char* argv[]);

    /// True if ANSI terminal execution mode is enabled.
    [[nodiscard]] bool is_ansi() const { return flags_.mode().is_ansi(); }
    /// True if PPM frame output mode is enabled.
    [[nodiscard]] bool is_ppm() const { return flags_.mode().is_ppm(); }
    /// True if diagnostic debug mode is enabled.
    [[nodiscard]] bool is_debug() const { return flags_.debug().is_debug(); }
    /// True if godmode / invincibility is enabled.
    [[nodiscard]] bool is_godmode() const { return flags_.debug().is_godmode(); }
    /// Random seed for obstacle spawner.
    [[nodiscard]] uint32_t seed() const { return limits_.seed(); }
    /// Maximum frames before exiting (-1 for unlimited).
    [[nodiscard]] int32_t max_frames() const { return limits_.max_frames(); }

    void set_ansi(bool val) { flags_.mode().set_ansi(val); }
    void set_ppm(bool val) { flags_.mode().set_ppm(val); }
    void set_debug(bool val) { flags_.debug().set_debug(val); }
    void set_godmode(bool val) { flags_.debug().set_godmode(val); }
    void set_seed(uint32_t val) { limits_.set_seed(val); }
    void set_max_frames(int32_t val) { limits_.set_max_frames(val); }

private:
    ApplicationFlags flags_;
    ExecutionLimits limits_;
};

}  // namespace dino::application

#endif  // DINO_APPLICATION_APPLICATION_OPTIONS_H_
