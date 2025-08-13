#include "sensors/ada254.h"

#include <driver/i2c_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <cstdarg>

#include "Arduino.h"
#include "driver/sdspi_host.h"
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "hal/spi_types.h"
#include "logger/logger.h"
#include "pgmspace.h"
#include "protocols/spi_interface.h"
#include "sd_protocol_types.h"

namespace sensors {
    ADA254::ADA254(spi::SPI_Config cfg) : spi::SPI_Interface(cfg) {
        sdspi_device_config_t slot_cfg{
            .host_id = SPI3_HOST,
            .gpio_cs = cfg.cs,
            .gpio_cd = GPIO_NUM_NC,
            .gpio_wp = GPIO_NUM_NC,
            .gpio_int = GPIO_NUM_NC,
        };

        sdmmc_host_t host = SDSPI_HOST_DEFAULT();
        host.slot = SPI3_HOST;

        esp_vfs_fat_sdmmc_mount_config_t mount_cfg = {
            .format_if_mount_failed = true,
            .max_files = 10,
            .allocation_unit_size = 0,
        };

        if (auto ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_cfg, &mount_cfg, &card)) {
            D_LOGW(cfg.name.c_str(), "Failed to mount SD Card %s", esp_err_to_name(ret));
        }

        init();
    }

    ADA254::~ADA254() {
        fclose(orient);
        fclose(temp);
        fclose(pressure);
        fclose(humidity);
        esp_vfs_fat_sdcard_unmount("/sdcard", card);
    }

    void ADA254::write(FileName file, const char* fmt, ...) {
        char data[256];
        va_list args;
        va_start(args, fmt);
        vsnprintf(data, sizeof(data), fmt, args);
        va_end(args);

        switch (file) {
            case (FileName::ORIENT):
                orient = fopen("/sdcard/orient.txt", "a");
                fprintf(orient, data);
                fclose(orient);
                break;
            case (FileName::TEMP):
                temp = fopen("/sdcard/temp.txt", "a");
                fprintf(temp, data);
                fclose(temp);
                break;
            case (FileName::PRESSURE):
                pressure = fopen("/sdcard/pressure.txt", "a");
                fprintf(pressure, data);
                fclose(pressure);
                break;
            case (FileName::HUMIDITY):
                humidity = fopen("/sdcard/humidity.txt", "a");
                fprintf(humidity, data);
                fclose(humidity);
                break;
            default:
                break;
        };
    }

    esp_err_t ADA254::init() {
        // Open all files with "W" to clear them
        orient = fopen("/sdcard/orient.txt", "w");
        temp = fopen("/sdcard/temp.txt", "w");
        pressure = fopen("/sdcard/pressure.txt", "w");
        humidity = fopen("/sdcard/humidity.txt", "w");
        return ESP_OK;
    }
}  // namespace sensors
