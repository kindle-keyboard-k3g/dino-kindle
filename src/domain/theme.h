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

    [[nodiscard]] constexpr bool is_night() const { return is_night_; }
    [[nodiscard]] constexpr bool is_milestone_active() const {
        return milestone_active_;
    }

    [[nodiscard]] constexpr ColorValue background_color() const {
        if (is_night_) {
            return ColorValue::Black;
        }
        return ColorValue::White;
    }

    [[nodiscard]] constexpr ColorValue foreground_color() const {
        if (is_night_) {
            return ColorValue::White;
        }
        return ColorValue::Black;
    }

    void toggle_mode() {
        is_night_ = !is_night_;
    }

    void set_milestone_active(bool active) {
        milestone_active_ = active;
    }

private:
    bool is_night_;
    bool milestone_active_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_THEME_H_
