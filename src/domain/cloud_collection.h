#ifndef DINO_DOMAIN_CLOUD_COLLECTION_H_
#define DINO_DOMAIN_CLOUD_COLLECTION_H_

#include "domain/cloud.h"

#include <array>
#include <cstddef>

namespace dino::domain {

/**
 * @brief First-class collection for decorative clouds.
 */
class CloudCollection {
public:
    static constexpr size_t MAX_CAPACITY = 6;

    constexpr CloudCollection() : items_{}, count_(0) {}

    /**
     * @brief Number of active clouds currently tracked.
     * @return Count of items in collection.
     */
    [[nodiscard]] constexpr size_t count() const { return count_; }

    /**
     * @brief Checks if collection has zero clouds.
     * @return True if empty.
     */
    [[nodiscard]] constexpr bool is_empty() const { return count_ == 0; }

    /**
     * @brief Checks if collection reached maximum capacity.
     * @return True if full.
     */
    [[nodiscard]] constexpr bool is_full() const { return count_ >= MAX_CAPACITY; }

    /**
     * @brief Gets reference to cloud at specified index.
     * @param index Zero-based item index.
     * @return Const reference to Cloud.
     */
    [[nodiscard]] const Cloud& at(size_t index) const;

    /**
     * @brief Appends a cloud if capacity allows.
     * @param cloud Cloud instance to append.
     */
    void add(const Cloud& cloud);

    /**
     * @brief Updates positions of all clouds in the collection.
     * @param speed Current world scroll velocity.
     */
    void update_all(FixedPoint speed);

    /**
     * @brief Removes all clouds that moved off-screen.
     */
    void prune_off_screen();

    /**
     * @brief Clears all clouds.
     */
    void clear();

private:
    void remove_at(size_t index);

    std::array<Cloud, MAX_CAPACITY> items_;
    size_t count_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_CLOUD_COLLECTION_H_
