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

    [[nodiscard]] bool is_frozen() const { return freeze_frames_remaining_ > 0; }
    [[nodiscard]] RefreshWaveform determine_waveform() const;

    void notify_milestone();
    void notify_game_over();
    void notify_restart();
    void request_full_refresh();
    void acknowledge_refresh();
    void decrement_freeze();

private:
    bool full_refresh_requested_;
    int32_t freeze_frames_remaining_;
};

}  // namespace dino::graphics

#endif  // DINO_GRAPHICS_REFRESH_COORDINATOR_H_
