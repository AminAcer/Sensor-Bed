#include "protocols/spi_interface.h"

#include <logger/logger.h>

namespace spi {
    const static char* TAG = "SPI";

    SPI_Interface::SPI_Interface(SPI_Config cfg) : name(cfg.name) {
        spi_device_interface_config_t dev_cfg{
            .mode = 0,
            .clock_speed_hz = cfg.speed_hz,
            .spics_io_num = cfg.cs,
            .queue_size = 1,
        };

        if (spi_bus_add_device(SPI3_HOST, &dev_cfg, &dev_handle)) {
            D_LOGW(TAG, "SPI failed to add device: %s", cfg.name.c_str());
        }
    }

    esp_err_t init_spi_master_bus(gpio_num_t miso_port, gpio_num_t mosi_port, gpio_num_t clk_port,
                                  spi_host_device_t spi_host) {
        spi_bus_config_t bus_config{
            .mosi_io_num = mosi_port,
            .miso_io_num = miso_port,
            .sclk_io_num = clk_port,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 4096,
        };

        if (spi_bus_initialize(SPI3_HOST, &bus_config, SPI_DMA_CH_AUTO)) {
            D_LOGW(TAG, "SPI failed to initialize");
            return ESP_FAIL;
        }

        return ESP_OK;
    }
}  // namespace spi
