#include "test_framework.h"
#include "graphics/refresh_coordinator.h"

using namespace dino;

DINO_TEST_CASE(TestRefreshCoordinatorWaveforms) {
    graphics::RefreshCoordinator coord;
    // Startup must be FullGrayscaleClear
    ASSERT_EQ(coord.determine_waveform(), graphics::RefreshWaveform::FullGrayscaleClear);

    coord.acknowledge_refresh();
    // After acknowledgment, regular play is PartialDirectUpdate
    ASSERT_EQ(coord.determine_waveform(), graphics::RefreshWaveform::PartialDirectUpdate);

    // Game over re-triggers FullGrayscaleClear
    coord.notify_game_over();
    ASSERT_EQ(coord.determine_waveform(), graphics::RefreshWaveform::FullGrayscaleClear);
}

DINO_TEST_CASE(TestRefreshCoordinatorMilestoneFreeze) {
    graphics::RefreshCoordinator coord;
    coord.acknowledge_refresh();

    coord.notify_milestone();
    ASSERT_TRUE(coord.is_frozen());
    ASSERT_EQ(coord.determine_waveform(), graphics::RefreshWaveform::FullGrayscaleClear);

    for (int i = 0; i < graphics::RefreshCoordinator::MILESTONE_FREEZE_FRAMES; ++i) {
        ASSERT_TRUE(coord.is_frozen());
        coord.decrement_freeze();
    }
    ASSERT_FALSE(coord.is_frozen());
}
