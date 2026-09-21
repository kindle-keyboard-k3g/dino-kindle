#include "test_framework.h"
#include "graphics/canvas.h"

using namespace dino;

DINO_TEST_CASE(TestCanvasPixelOperations) {
    graphics::Canvas canvas(domain::Dimensions(20, 20));
    canvas.clear(domain::ColorValue::White);
    ASSERT_EQ(canvas.get_pixel(5, 5), domain::ColorValue::White);

    canvas.set_pixel(5, 5, domain::ColorValue::Black);
    ASSERT_EQ(canvas.get_pixel(5, 5), domain::ColorValue::Black);

    // Out of bounds set and get do not crash
    canvas.set_pixel(-1, 5, domain::ColorValue::Black);
    canvas.set_pixel(50, 50, domain::ColorValue::Black);
    ASSERT_EQ(canvas.get_pixel(-1, 5), domain::ColorValue::White);
}

DINO_TEST_CASE(TestCanvasFillRect) {
    graphics::Canvas canvas(domain::Dimensions(20, 20));
    canvas.clear(domain::ColorValue::White);

    domain::BoundingBox box(domain::ScreenCoordinate(2, 2), domain::Dimensions(3, 3));
    canvas.fill_rect(box, domain::ColorValue::Black);

    ASSERT_EQ(canvas.get_pixel(2, 2), domain::ColorValue::Black);
    ASSERT_EQ(canvas.get_pixel(4, 4), domain::ColorValue::Black);
    ASSERT_EQ(canvas.get_pixel(5, 5), domain::ColorValue::White); // Exclusive bound
}
