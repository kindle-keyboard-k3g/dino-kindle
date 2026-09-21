#include "domain/cloud_collection.h"

#include <stdexcept>

namespace dino::domain {

const Cloud& CloudCollection::at(size_t index) const {
    if (index >= count_) {
        throw std::out_of_range("CloudCollection index out of range");
    }
    return items_[index];
}

void CloudCollection::add(const Cloud& cloud) {
    if (is_full()) {
        return;
    }
    items_[count_] = cloud;
    count_++;
}

void CloudCollection::remove_at(size_t index) {
    if (index >= count_) {
        return;
    }
    for (size_t i = index; i + 1 < count_; ++i) {
        items_[i] = items_[i + 1];
    }
    count_--;
}

void CloudCollection::update_all(FixedPoint speed) {
    for (size_t i = 0; i < count_; ++i) {
        items_[i].update(speed);
    }
}

void CloudCollection::prune_off_screen() {
    size_t i = 0;
    while (i < count_) {
        if (items_[i].is_off_screen()) {
            remove_at(i);
            continue;
        }
        ++i;
    }
}

void CloudCollection::clear() {
    count_ = 0;
}

}  // namespace dino::domain
