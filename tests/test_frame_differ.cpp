#include "test_framework.h"
#include "graphics/frame_differ.h"

using namespace dino;

DINO_TEST_CASE(TestFrameDifferIdenticalFrames) {
    graphics::Canvas a(domain::Dimensions(20, 20));
    graphics::Canvas b(domain::Dimensions(20, 20));
    a.clear(domain::ColorValue::White);
    b.clear(domain::ColorValue::White);

    domain::BoundingBox damage = graphics::FrameDiffer::compute_damage(a, b);
    ASSERT_TRUE(damage.is_empty());
}

DINO_TEST_CASE(TestFrameDifferChangedRegion) {
    graphics::Canvas a(domain::Dimensions(40, 40));
    graphics::Canvas b(domain::Dimensions(40, 40));
    a.clear(domain::ColorValue::White);
    b.clear(domain::ColorValue::White);

    // Modify pixels in b at (10, 15) and (20, 25)
    b.set_pixel(10, 15, domain::ColorValue::Black);
    b.set_pixel(20, 25, domain::ColorValue::Black);

    domain::BoundingBox damage = graphics::FrameDiffer::compute_damage(a, b);
    ASSERT_FALSE(damage.is_empty());
    ASSERT_EQ(damage.left(), 10);
    ASSERT_EQ(damage.top(), 15);
    ASSERT_EQ(damage.right(), 21); // exclusive: 20 + 1
    ASSERT_EQ(damage.bottom(), 26); // exclusive: 25 + 1
}
