#include "domain/obstacle_collection.h"

#include <stdexcept>

namespace dino::domain {

const Obstacle& ObstacleCollection::at(size_t index) const {
    if (index >= count_) {
        throw std::out_of_range("ObstacleCollection index out of range");
    }
    return items_[index];
}

void ObstacleCollection::add(const Obstacle& obstacle) {
    if (is_full()) {
        return;
    }
    items_[count_] = obstacle;
    count_++;
}

void ObstacleCollection::remove_at(size_t index) {
    if (index >= count_) {
        return;
    }
    for (size_t i = index; i + 1 < count_; ++i) {
        items_[i] = items_[i + 1];
    }
    count_--;
}

void ObstacleCollection::update_all(FixedPoint speed) {
    for (size_t i = 0; i < count_; ++i) {
        items_[i].update(speed);
    }
}

void ObstacleCollection::prune_off_screen() {
    size_t i = 0;
    while (i < count_) {
        if (items_[i].is_off_screen()) {
            remove_at(i);
            continue;
        }
        ++i;
    }
}

void ObstacleCollection::clear() {
    count_ = 0;
}

}  // namespace dino::domain
