#include "test_framework.h"
#include "domain/geometry.h"

using namespace dino::domain;

DINO_TEST_CASE(TestScreenCoordinateAndDimensions) {
    ScreenCoordinate coord(10, 20);
    ASSERT_EQ(coord.x(), 10);
    ASSERT_EQ(coord.y(), 20);

    Dimensions dims(100, 200);
    ASSERT_EQ(dims.width(), 100);
    ASSERT_EQ(dims.height(), 200);
    ASSERT_FALSE(dims.is_empty());

    Dimensions empty_dims(0, 50);
    ASSERT_TRUE(empty_dims.is_empty());
}

DINO_TEST_CASE(TestBoundingBoxExclusiveBounds) {
    BoundingBox box(ScreenCoordinate(50, 100), Dimensions(40, 60));
    ASSERT_EQ(box.left(), 50);
    ASSERT_EQ(box.top(), 100);
    ASSERT_EQ(box.right(), 90);   // exclusive: 50 + 40
    ASSERT_EQ(box.bottom(), 160); // exclusive: 100 + 60
    ASSERT_EQ(box.width(), 40);
    ASSERT_EQ(box.height(), 60);
    ASSERT_FALSE(box.is_empty());
}

DINO_TEST_CASE(TestBoundingBoxIntersection) {
    BoundingBox a(ScreenCoordinate(10, 10), Dimensions(20, 20)); // [10, 10] to [30, 30]
    BoundingBox b(ScreenCoordinate(25, 25), Dimensions(20, 20)); // [25, 25] to [45, 45]
    BoundingBox c(ScreenCoordinate(30, 30), Dimensions(20, 20)); // Touching edge at 30
    BoundingBox d(ScreenCoordinate(50, 50), Dimensions(20, 20)); // Completely disjoint

    ASSERT_TRUE(a.intersects(b));
    ASSERT_FALSE(a.intersects(c)); // Exclusive bound means touching does not intersect
    ASSERT_FALSE(a.intersects(d));
}

DINO_TEST_CASE(TestBoundingBoxExpansionAndInset) {
    BoundingBox box(ScreenCoordinate(20, 30), Dimensions(40, 50));
    BoundingBox expanded = box.expanded(2);
    ASSERT_EQ(expanded.left(), 18);
    ASSERT_EQ(expanded.top(), 28);
    ASSERT_EQ(expanded.width(), 44);
    ASSERT_EQ(expanded.height(), 54);

    BoundingBox inset = box.inset(3);
    ASSERT_EQ(inset.left(), 23);
    ASSERT_EQ(inset.top(), 33);
    ASSERT_EQ(inset.width(), 34);
    ASSERT_EQ(inset.height(), 44);
}

DINO_TEST_CASE(TestBoundingBoxMergeAndClamping) {
    BoundingBox a(ScreenCoordinate(10, 10), Dimensions(20, 20));
    BoundingBox b(ScreenCoordinate(25, 25), Dimensions(20, 20));
    BoundingBox merged = a.merged_with(b);
    ASSERT_EQ(merged.left(), 10);
    ASSERT_EQ(merged.top(), 10);
    ASSERT_EQ(merged.right(), 45);
    ASSERT_EQ(merged.bottom(), 45);

    BoundingBox screen(ScreenCoordinate(0, 0), Dimensions(100, 100));
    BoundingBox offscreen(ScreenCoordinate(80, 80), Dimensions(50, 50));
    BoundingBox clamped = offscreen.clamped_to(screen);
    ASSERT_EQ(clamped.left(), 80);
    ASSERT_EQ(clamped.top(), 80);
    ASSERT_EQ(clamped.right(), 100);
    ASSERT_EQ(clamped.bottom(), 100);
}
