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

    /**
     * @brief Horizontal screen position.
     * @return X coordinate in pixels.
     */
    [[nodiscard]] constexpr int32_t x() const { return x_; }

    /**
     * @brief Vertical screen position.
     * @return Y coordinate in pixels.
     */
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

    /**
     * @brief Width extent in pixels.
     * @return Width value.
     */
    [[nodiscard]] constexpr int32_t width() const { return width_; }

    /**
     * @brief Height extent in pixels.
     * @return Height value.
     */
    [[nodiscard]] constexpr int32_t height() const { return height_; }

    /**
     * @brief Checks whether dimensions represent an empty or non-positive area.
     * @return True if width <= 0 or height <= 0.
     */
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

    /**
     * @brief Left boundary X coordinate.
     * @return Minimum X in pixels.
     */
    [[nodiscard]] constexpr int32_t left() const { return origin_.x(); }

    /**
     * @brief Top boundary Y coordinate.
     * @return Minimum Y in pixels.
     */
    [[nodiscard]] constexpr int32_t top() const { return origin_.y(); }

    /**
     * @brief Right boundary X coordinate (exclusive).
     * @return origin.x + width.
     */
    [[nodiscard]] constexpr int32_t right() const {
        return origin_.x() + dimensions_.width();
    }

    /**
     * @brief Bottom boundary Y coordinate (exclusive).
     * @return origin.y + height.
     */
    [[nodiscard]] constexpr int32_t bottom() const {
        return origin_.y() + dimensions_.height();
    }

    /**
     * @brief Box width in pixels.
     * @return Dimensions width.
     */
    [[nodiscard]] constexpr int32_t width() const { return dimensions_.width(); }

    /**
     * @brief Box height in pixels.
     * @return Dimensions height.
     */
    [[nodiscard]] constexpr int32_t height() const { return dimensions_.height(); }

    /**
     * @brief Checks if bounding box encloses zero area.
     * @return True if width <= 0 or height <= 0.
     */
    [[nodiscard]] constexpr bool is_empty() const { return dimensions_.is_empty(); }

    /**
     * @brief Tests intersection with another bounding box.
     * @param other BoundingBox to check.
     * @return True if boxes overlap.
     */
    [[nodiscard]] bool intersects(const BoundingBox& other) const;

    /**
     * @brief Expands box uniformly in all four directions.
     * @param margin Pixels to expand outward.
     * @return Enlarged BoundingBox.
     */
    [[nodiscard]] BoundingBox expanded(int32_t margin) const;

    /**
     * @brief Insets box inward uniformly.
     * @param margin Pixels to shrink inward.
     * @return Shrunk BoundingBox.
     */
    [[nodiscard]] BoundingBox inset(int32_t margin) const;

    /**
     * @brief Computes smallest bounding box containing both boxes.
     * @param other Box to union with.
     * @return Unified BoundingBox.
     */
    [[nodiscard]] BoundingBox merged_with(const BoundingBox& other) const;

    /**
     * @brief Clamps box boundaries within specified enclosing bounds.
     * @param bounds Outer bounding rectangle.
     * @return Clamped BoundingBox.
     */
    [[nodiscard]] BoundingBox clamped_to(const BoundingBox& bounds) const;

private:
    ScreenCoordinate origin_;
    Dimensions dimensions_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_GEOMETRY_H_
