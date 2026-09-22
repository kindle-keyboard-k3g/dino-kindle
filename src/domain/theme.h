#ifndef DINO_DOMAIN_THEME_H_
#define DINO_DOMAIN_THEME_H_

#include <cstdint>

namespace dino::domain {

/**
 * @brief E-Ink Grayscale palette polarities (0x0 = Pure White, 0xF = Pure Black).
 */
enum class ColorValue : uint8_t {
    White = 0x00,
    LightGray = 0x05,
    DarkGray = 0x0A,
    Black = 0x0F
};

/**
 * @brief Manages day/night palette and milestone display inversion.
 */
class Theme {
public:
    constexpr Theme() : is_night_(false), milestone_active_(false) {}

    /**
     * @brief Checks if night mode is currently active.
     * @return True if dark background is enabled.
     */
    [[nodiscard]] constexpr bool is_night() const { return is_night_; }

    /**
     * @brief Checks if score milestone inverted flash is active.
     * @return True if milestone effect is playing.
     */
    [[nodiscard]] constexpr bool is_milestone_active() const {
        return milestone_active_;
    }

    /**
     * @brief Determines background color based on day/night mode.
     * @return ColorValue for background clear.
     */
    [[nodiscard]] constexpr ColorValue background_color() const {
        if (is_night_) {
            return ColorValue::Black;
        }
        return ColorValue::White;
    }

    /**
     * @brief Determines foreground drawing color based on day/night mode.
     * @return ColorValue for sprites and text.
     */
    [[nodiscard]] constexpr ColorValue foreground_color() const {
        if (is_night_) {
            return ColorValue::White;
        }
        return ColorValue::Black;
    }

    /**
     * @brief Inverts between day (white background) and night (black background).
     */
    void toggle_mode() {
        is_night_ = !is_night_;
    }

    /**
     * @brief Toggles milestone flash effect.
     * @param active True to enable flash, false to disable.
     */
    void set_milestone_active(bool active) {
        milestone_active_ = active;
    }

private:
    bool is_night_;
    bool milestone_active_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_THEME_H_
