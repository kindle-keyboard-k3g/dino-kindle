#include "graphics/refresh_coordinator.h"

namespace dino::graphics {

RefreshCoordinator::RefreshCoordinator()
    : full_refresh_requested_(true),
      freeze_frames_remaining_(0) {}

RefreshWaveform RefreshCoordinator::determine_waveform() const {
    if (full_refresh_requested_) {
        return RefreshWaveform::FullGrayscaleClear;
    }
    return RefreshWaveform::PartialDirectUpdate;
}

void RefreshCoordinator::notify_milestone() {
    full_refresh_requested_ = true;
    freeze_frames_remaining_ = MILESTONE_FREEZE_FRAMES;
}

void RefreshCoordinator::notify_game_over() {
    full_refresh_requested_ = true;
}

void RefreshCoordinator::notify_restart() {
    full_refresh_requested_ = true;
    freeze_frames_remaining_ = 0;
}

void RefreshCoordinator::request_full_refresh() {
    full_refresh_requested_ = true;
}

void RefreshCoordinator::acknowledge_refresh() {
    full_refresh_requested_ = false;
}

void RefreshCoordinator::decrement_freeze() {
    if (freeze_frames_remaining_ > 0) {
        freeze_frames_remaining_--;
    }
}

}  // namespace dino::graphics
