#include "graphics/dirty_region_tracker.h"

#include <stdexcept>

namespace dino::graphics {

const domain::BoundingBox& DirtyRegionTracker::at(size_t index) const {
    if (index >= count_) {
        throw std::out_of_range("DirtyRegionTracker index out of range");
    }
    return regions_[index];
}

void DirtyRegionTracker::add_region(const domain::BoundingBox& box) {
    if (box.is_empty()) {
        return;
    }
    const domain::BoundingBox expanded_box = box.expanded(GHOSTING_MARGIN);
    if (count_ >= MAX_REGIONS) {
        regions_[0] = regions_[0].merged_with(expanded_box);
        return;
    }
    regions_[count_] = expanded_box;
    count_++;
    merge_overlapping();
}

void DirtyRegionTracker::add_full_screen(int32_t width, int32_t height) {
    clear();
    regions_[0] = domain::BoundingBox(
        domain::ScreenCoordinate(0, 0),
        domain::Dimensions(width, height));
    count_ = 1;
}

void DirtyRegionTracker::clear() {
    count_ = 0;
}

void DirtyRegionTracker::merge_overlapping() {
    if (count_ < 2) {
        return;
    }
    for (size_t i = 0; i < count_; ++i) {
        for (size_t j = i + 1; j < count_; ++j) {
            if (regions_[i].intersects(regions_[j])) {
                regions_[i] = regions_[i].merged_with(regions_[j]);
                regions_[j] = regions_[count_ - 1];
                count_--;
                return;
            }
        }
    }
}

}  // namespace dino::graphics
