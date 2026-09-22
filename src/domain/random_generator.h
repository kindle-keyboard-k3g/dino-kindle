#ifndef DINO_DOMAIN_RANDOM_GENERATOR_H_
#define DINO_DOMAIN_RANDOM_GENERATOR_H_

#include <cstdint>

namespace dino::domain {

/**
 * @brief Fast, deterministic 32-bit Linear Congruential Generator.
 */
class RandomGenerator {
public:
    explicit constexpr RandomGenerator(uint32_t seed = 0x12345678U)
        : state_(seed == 0 ? 0x12345678U : seed) {}

    /**
     * @brief Generates the next pseudorandom unsigned 32-bit integer.
     * @return Pseudorandom uint32_t value.
     */
    [[nodiscard]] uint32_t next_u32();

    /**
     * @brief Generates a random integer within [min_inclusive, max_inclusive].
     * @param min_inclusive Lower bound.
     * @param max_inclusive Upper bound.
     * @return Integer within the specified range.
     */
    [[nodiscard]] int32_t next_range(int32_t min_inclusive, int32_t max_inclusive);

private:
    uint32_t state_;
};

}  // namespace dino::domain

#endif  // DINO_DOMAIN_RANDOM_GENERATOR_H_
