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

    [[nodiscard]] constexpr size_t count() const { return count_; }
    [[nodiscard]] constexpr bool is_empty() const { return count_ == 0; }
    [[nodiscard]] constexpr bool is_full() const { return count_ >= MAX_CAPACITY; }

    [[nodiscard]] const Cloud& at(size_t index) const;
    void add(const Cloud& cloud);
    void update_all(FixedPoint speed);
    void prune_off_screen();
    void clear();

private:
    void remove_at(size_t index);

    std::array<Cloud, MAX_CAPACITY> items_;
    size_t count_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_CLOUD_COLLECTION_H_
