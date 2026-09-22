#include "test_framework.h"
#include "application/application_options.h"

using namespace dino::application;

DINO_TEST_CASE(TestApplicationOptionsDefault) {
    char* argv[] = {const_cast<char*>("dino")};
    const auto options = ApplicationOptions::parse(1, argv);
    ASSERT_FALSE(options.is_ansi());
    ASSERT_FALSE(options.is_ppm());
    ASSERT_FALSE(options.is_debug());
    ASSERT_FALSE(options.is_godmode());
    ASSERT_EQ(options.max_frames(), -1);
}

DINO_TEST_CASE(TestApplicationOptionsDebugAndGodmode) {
    char* argv[] = {
        const_cast<char*>("dino"),
        const_cast<char*>("--debug"),
        const_cast<char*>("--godmode")
    };
    const auto options = ApplicationOptions::parse(3, argv);
    ASSERT_TRUE(options.is_debug());
    ASSERT_TRUE(options.is_godmode());
}

DINO_TEST_CASE(TestApplicationOptionsShortFlags) {
    char* argv[] = {
        const_cast<char*>("dino"),
        const_cast<char*>("-d"),
        const_cast<char*>("-g")
    };
    const auto options = ApplicationOptions::parse(3, argv);
    ASSERT_TRUE(options.is_debug());
    ASSERT_TRUE(options.is_godmode());
}
