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

    /**
     * @brief Opens the specified framebuffer device node for EPDC ioctl calls.
     * @param device_path Path to framebuffer device (default "/dev/fb0").
     */
    explicit EinkControllerMxc(const char* device_path = "/dev/fb0");

    /**
     * @brief Attaches to an existing open file descriptor without taking ownership.
     * @param existing_fd Open file descriptor.
     */
    explicit EinkControllerMxc(int existing_fd);
    ~EinkControllerMxc() override;

    EinkControllerMxc(const EinkControllerMxc&) = delete;
    EinkControllerMxc& operator=(const EinkControllerMxc&) = delete;

    /**
     * @brief Issues FBIO_EINK_UPDATE_DISPLAY_AREA ioctl over damaged region.
     * @param area Screen damage bounding box.
     * @param waveform Waveform mode (DirectUpdate or FullGrayscaleClear).
     */
    void update(const domain::BoundingBox& area,
                graphics::RefreshWaveform waveform) override;

private:
    int file_descriptor_;
    bool owns_descriptor_;
};

}  // namespace dino::hal

#endif  // DINO_HAL_EINK_CONTROLLER_MXC_H_
