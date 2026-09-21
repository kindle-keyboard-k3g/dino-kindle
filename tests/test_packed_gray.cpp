#include "test_framework.h"
#include "hal/packed_gray.h"

#include <vector>

using namespace dino;

DINO_TEST_CASE(TestPackedPixelPairNibbles) {
    uint8_t byte1 = hal::PackedGray::pack_pixel_pair(0x0A, 0x05);
    ASSERT_EQ(byte1, 0xA5);

    uint8_t byte2 = hal::PackedGray::pack_pixel_pair(0x00, 0x0F);
    ASSERT_EQ(byte2, 0x0F);

    uint8_t byte3 = hal::PackedGray::pack_pixel_pair(0x0F, 0x00);
    ASSERT_EQ(byte3, 0xF0);
}

DINO_TEST_CASE(TestPackFrameTotalBytes) {
    graphics::Canvas canvas; // Default 600x800
    canvas.clear(domain::ColorValue::White);

    std::vector<uint8_t> fb(hal::PackedGray::KINDLE_FB_SIZE, 0xFF);
    hal::PackedGray::pack_frame(canvas, fb.data());

    // All white pixels should pack to 0x00 bytes
    ASSERT_EQ(fb.size(), 240000);
    for (size_t i = 0; i < fb.size(); ++i) {
        ASSERT_EQ(fb[i], 0x00);
    }
}
