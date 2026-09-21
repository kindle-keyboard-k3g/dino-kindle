#ifndef DINO_HAL_EINK_CONTROLLER_MXC_H_
#define DINO_HAL_EINK_CONTROLLER_MXC_H_

#include "hal/eink_controller.h"

namespace dino::hal {

/**
 * @brief Kindle i.MX35 EPDC hardware controller executing ioctl 0x46dd.
 */
class EinkControllerMxc : public EinkController {
public:
    static constexpr unsigned long FBIO_EINK_UPDATE_DISPLAY_AREA = 0x46dd;

    explicit EinkControllerMxc(const char* device_path = "/dev/fb0");
    explicit EinkControllerMxc(int existing_fd);
    ~EinkControllerMxc() override;

    EinkControllerMxc(const EinkControllerMxc&) = delete;
    EinkControllerMxc& operator=(const EinkControllerMxc&) = delete;

    void update(const domain::BoundingBox& area,
                graphics::RefreshWaveform waveform) override;

private:
    int file_descriptor_;
    bool owns_descriptor_;
};

}  // namespace dino::hal

#endif  // DINO_HAL_EINK_CONTROLLER_MXC_H_
