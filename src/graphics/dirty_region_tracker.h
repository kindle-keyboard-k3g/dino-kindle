#ifndef DINO_GRAPHICS_DIRTY_REGION_TRACKER_H_
#define DINO_GRAPHICS_DIRTY_REGION_TRACKER_H_

#include "domain/geometry.h"

#include <array>
#include <cstddef>

namespace dino::graphics {

/**
 * @brief Manages damage bounding boxes with ghosting-mitigation margins.
 */
class DirtyRegionTracker {
public:
    static constexpr int32_t GHOSTING_MARGIN = 2;
    static constexpr size_t MAX_REGIONS = 4;

    DirtyRegionTracker() : regions_{}, count_(0) {}

    [[nodiscard]] size_t count() const { return count_; }
    [[nodiscard]] const domain::BoundingBox& at(size_t index) const;

    void add_region(const domain::BoundingBox& box);
    void add_full_screen(int32_t width, int32_t height);
    void clear();
    void merge_overlapping();

private:
    std::array<domain::BoundingBox, MAX_REGIONS> regions_;
    size_t count_;
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_DIRTY_REGION_TRACKER_H_
