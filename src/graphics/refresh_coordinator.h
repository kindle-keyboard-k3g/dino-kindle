#ifndef DINO_GRAPHICS_REFRESH_COORDINATOR_H_
#define DINO_GRAPHICS_REFRESH_COORDINATOR_H_

#include <cstdint>

namespace dino::graphics {

enum class RefreshWaveform : uint8_t {
    PartialDirectUpdate,  // DU mode (~100ms)
    FullGrayscaleClear    // GC16 mode (~450ms)
};

/**
 * @brief Coordinates DU vs GC16 waveforms and milestone anti-ghosting freezes.
 */
class RefreshCoordinator {
public:
    static constexpr int32_t MILESTONE_FREEZE_FRAMES = 6;  // ~500ms at 80ms/frame

    RefreshCoordinator();

    /// Indicates if milestone freeze is currently active.
    [[nodiscard]] bool is_frozen() const { return freeze_frames_remaining_ > 0; }
    /// Determines whether to issue PartialDirectUpdate or FullGrayscaleClear.
    [[nodiscard]] RefreshWaveform determine_waveform() const;

    /// Triggers milestone freeze and schedule full refresh.
    void notify_milestone();
    /// Schedules full clear refresh on game over.
    void notify_game_over();
    /// Clears freeze and schedules clean refresh on restart.
    void notify_restart();
    /// Manually requests a full GC16 refresh.
    void request_full_refresh();
    /// Acknowledges execution of full refresh waveform.
    void acknowledge_refresh();
    /// Decrements milestone freeze counter by one frame.
    void decrement_freeze();

private:
    bool full_refresh_requested_;
    int32_t freeze_frames_remaining_;
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_REFRESH_COORDINATOR_H_
