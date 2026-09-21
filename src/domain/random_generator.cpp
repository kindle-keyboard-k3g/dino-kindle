#include "domain/random_generator.h"

#include <stdexcept>

namespace dino::domain {

uint32_t RandomGenerator::next_u32() {
    state_ = state_ * 1664525U + 1013904223U;
    return state_;
}

int32_t RandomGenerator::next_range(int32_t min_inclusive, int32_t max_inclusive) {
    if (min_inclusive > max_inclusive) {
        throw std::invalid_argument("min_inclusive cannot exceed max_inclusive");
    }
    if (min_inclusive == max_inclusive) {
        return min_inclusive;
    }
    const uint32_t span = static_cast<uint32_t>(max_inclusive - min_inclusive + 1);
    const uint32_t random_val = next_u32() % span;
    return min_inclusive + static_cast<int32_t>(random_val);
}

}  // namespace dino::domain
