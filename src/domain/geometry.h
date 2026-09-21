#ifndef DINO_DOMAIN_GEOMETRY_H_
#define DINO_DOMAIN_GEOMETRY_H_

#include <cstdint>

namespace dino::domain {

/**
 * @brief Discrete 2D screen coordinate.
 */
class ScreenCoordinate {
public:
    constexpr ScreenCoordinate() : x_(0), y_(0) {}
    constexpr ScreenCoordinate(int32_t x, int32_t y) : x_(x), y_(y) {}

    [[nodiscard]] constexpr int32_t x() const { return x_; }
    [[nodiscard]] constexpr int32_t y() const { return y_; }

    constexpr bool operator==(const ScreenCoordinate& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }

private:
    int32_t x_;
    int32_t y_;
};

/**
 * @brief Discrete 2D dimensions.
 */
class Dimensions {
public:
    constexpr Dimensions() : width_(0), height_(0) {}
    constexpr Dimensions(int32_t width, int32_t height)
        : width_(width), height_(height) {}

    [[nodiscard]] constexpr int32_t width() const { return width_; }
    [[nodiscard]] constexpr int32_t height() const { return height_; }

    [[nodiscard]] constexpr bool is_empty() const {
        return width_ <= 0 || height_ <= 0;
    }

private:
    int32_t width_;
    int32_t height_;
};

/**
 * @brief 2D Axis-Aligned Bounding Box with exclusive boundaries (x2 = x + width).
 */
class BoundingBox {
public:
    constexpr BoundingBox() : origin_{}, dimensions_{} {}
    constexpr BoundingBox(ScreenCoordinate origin, Dimensions dimensions)
        : origin_(origin), dimensions_(dimensions) {}

    [[nodiscard]] constexpr int32_t left() const { return origin_.x(); }
    [[nodiscard]] constexpr int32_t top() const { return origin_.y(); }
    [[nodiscard]] constexpr int32_t right() const {
        return origin_.x() + dimensions_.width();
    }
    [[nodiscard]] constexpr int32_t bottom() const {
        return origin_.y() + dimensions_.height();
    }
    [[nodiscard]] constexpr int32_t width() const { return dimensions_.width(); }
    [[nodiscard]] constexpr int32_t height() const { return dimensions_.height(); }
    [[nodiscard]] constexpr bool is_empty() const { return dimensions_.is_empty(); }

    [[nodiscard]] bool intersects(const BoundingBox& other) const;
    [[nodiscard]] BoundingBox expanded(int32_t margin) const;
    [[nodiscard]] BoundingBox inset(int32_t margin) const;
    [[nodiscard]] BoundingBox merged_with(const BoundingBox& other) const;
    [[nodiscard]] BoundingBox clamped_to(const BoundingBox& bounds) const;

private:
    ScreenCoordinate origin_;
    Dimensions dimensions_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_GEOMETRY_H_
