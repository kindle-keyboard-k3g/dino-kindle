#ifndef DINO_DOMAIN_OBSTACLE_COLLECTION_H_
#define DINO_DOMAIN_OBSTACLE_COLLECTION_H_

#include "domain/obstacle.h"

#include <array>
#include <cstddef>

namespace dino::domain {

/**
 * @brief First-class fixed-capacity collection of active obstacles.
 */
class ObstacleCollection {
public:
    static constexpr size_t MAX_CAPACITY = 16;

    constexpr ObstacleCollection() : items_{}, count_(0) {}

    /**
     * @brief Number of active obstacles currently tracked.
     * @return Count of items in collection.
     */
    [[nodiscard]] constexpr size_t count() const { return count_; }

    /**
     * @brief Checks if collection contains zero obstacles.
     * @return True if empty.
     */
    [[nodiscard]] constexpr bool is_empty() const { return count_ == 0; }

    /**
     * @brief Checks if collection reached capacity.
     * @return True if full.
     */
    [[nodiscard]] constexpr bool is_full() const { return count_ >= MAX_CAPACITY; }

    /**
     * @brief Retrieves obstacle at index.
     * @param index Item index.
     * @return Const reference to Obstacle.
     */
    [[nodiscard]] const Obstacle& at(size_t index) const;

    /**
     * @brief Appends an obstacle if collection is not full.
     * @param obstacle Obstacle to insert.
     */
    void add(const Obstacle& obstacle);

    /**
     * @brief Updates positions of all active obstacles.
     * @param speed Current scroll velocity.
     */
    void update_all(FixedPoint speed);

    /**
     * @brief Removes obstacles that moved past left screen edge.
     */
    void prune_off_screen();

    /**
     * @brief Clears all obstacles.
     */
    void clear();

private:
    void remove_at(size_t index);

    std::array<Obstacle, MAX_CAPACITY> items_;
    size_t count_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_OBSTACLE_COLLECTION_H_
