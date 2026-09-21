#ifndef DINO_HAL_EINK_CONTROLLER_H_
#define DINO_HAL_EINK_CONTROLLER_H_

#include "domain/geometry.h"
#include "graphics/refresh_coordinator.h"

namespace dino::hal {

/**
 * @brief Abstract controller for electronic paper waveform execution.
 */
class EinkController {
public:
    virtual ~EinkController() = default;

    /**
     * @brief Dispatches screen refresh waveform over specified bounding area.
     */
    virtual void update(const domain::BoundingBox& area,
                        graphics::RefreshWaveform waveform) = 0;
};

}  // namespace dino::hal

#endif  // DINO_HAL_EINK_CONTROLLER_H_
