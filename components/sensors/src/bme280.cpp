#include "sensors/bme280.h"

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <cstdint>
#include <mutex>

/// @brief Control registers
#define BME280_CHIP_ID_ADDR 0xD0
#define BME280_RST_ADDR 0xE0
#define BME280_CTRL_HUM_ADDR 0xF2
#define BME280_CTRL_MEAS_ADDR 0xF4
#define BME280_CFG_ADDR 0xF5

/// @brief Data register starting points
#define BME280_HUM_MSB 0xFD    // Humidity starts here (2 bytes total, 15:0)
#define BME280_PRESS_MSB 0xF7  // Pressure starts here (3 bytes total, 19:0)
#define BME280_TEMP_MSB 0xFA   // Temperature starts here (3 bytes total, 19:0)

/// @brief Calibration register starting points
#define BME280_CALI_TEMP_PRESS \
    0x88                         // Calibration data for Temp/Pressure starts here (26 bytes total)
#define BME280_CALI_HUM_H1 0xA1  // H1 Calibration data (1 byte total)
#define BME280_CALI_HUM_H2_TO_H6 \
    0xE1  // H2, H3, H4, H5, H6 Calibration data starts here (7 bytes total)

/// @brief Chip ID
#define BME280_ID 0x60

namespace sensors {
    const static char* BME_TAG = "bme280";

    BME280::BME280(i2c::I2C_Config cfg, i2c_master_bus_handle_t bus_handle)
        : i2c::I2C_Interface(cfg, bus_handle) {
        init();
        // Read/Store calibration data for Compensation calculations
        read_calibration();
    }

    void BME280::run() {
        // Collect latest Humidity/Pressure/Temperature readings
        read_all();
        // Collect latest Humidity readings
        // read_humidity();
        // Collect latest Pressure readings
        // read_pressure();
        // Collect latest Temperature readings
        // read_temperature();
    }

    esp_err_t BME280::init() {
        uint8_t id = 0;
        ESP_ERROR_CHECK(read(BME280_CHIP_ID_ADDR, &id, 1));
        if (id != BME280_ID) {
            ESP_LOGE(BME_TAG, "Unexpected chip ID: 0x%02X", id);
            return ESP_FAIL;
        }

        // Set Humidity to oversampling 1
        ESP_ERROR_CHECK(write(BME280_CTRL_HUM_ADDR, 0x01));
        vTaskDelay(pdMS_TO_TICKS(25));

        // Set Temperature/Pressure oversampling and mode
        uint8_t ctrl_meas = (1 << 5) | (1 << 2) | 0x03;
        // 001        001        11
        //  ^          ^         ^^
        //  |          |         ||
        // osrs_t   osrs_p      mode
        ESP_ERROR_CHECK(write(BME280_CTRL_MEAS_ADDR, ctrl_meas));
        vTaskDelay(pdMS_TO_TICKS(25));

        // Set standby time, IIR filter, and disable 3 wire SPI mode (we're doing i2c)
        // 100        100        00
        //  ^          ^         ^^
        //  |          |         ||
        // t_sb     filter      spi3w_en
        uint8_t cfg = (4 << 5) | (4 << 2) | 0;
        ESP_ERROR_CHECK(write(BME280_CFG_ADDR, cfg));
        vTaskDelay(pdMS_TO_TICKS(25));

        return ESP_OK;
    }

    void BME280::read_calibration() {
        uint8_t tp_buffer[24];
        uint8_t h1_buffer[1];
        uint8_t h2h6_buffer[7];
        if (read(BME280_CALI_TEMP_PRESS, tp_buffer, 24) != ESP_OK ||
            read(BME280_CALI_HUM_H1, h1_buffer, 1) != ESP_OK ||
            read(BME280_CALI_HUM_H2_TO_H6, h2h6_buffer, 7) != ESP_OK) {
            return;
        }

        std::lock_guard lock(dataMutex);
        cali.dig_T1 = (uint16_t)(tp_buffer[1] << 8 | tp_buffer[0]);
        cali.dig_T2 = (int16_t)(tp_buffer[3] << 8 | tp_buffer[2]);
        cali.dig_T3 = (int16_t)(tp_buffer[5] << 8 | tp_buffer[4]);

        cali.dig_P1 = (uint16_t)(tp_buffer[7] << 8 | tp_buffer[6]);
        cali.dig_P2 = (int16_t)(tp_buffer[9] << 8 | tp_buffer[8]);
        cali.dig_P3 = (int16_t)(tp_buffer[11] << 8 | tp_buffer[10]);
        cali.dig_P4 = (int16_t)(tp_buffer[13] << 8 | tp_buffer[12]);
        cali.dig_P5 = (int16_t)(tp_buffer[15] << 8 | tp_buffer[14]);
        cali.dig_P6 = (int16_t)(tp_buffer[17] << 8 | tp_buffer[16]);
        cali.dig_P7 = (int16_t)(tp_buffer[19] << 8 | tp_buffer[18]);
        cali.dig_P8 = (int16_t)(tp_buffer[21] << 8 | tp_buffer[20]);
        cali.dig_P9 = (int16_t)(tp_buffer[23] << 8 | tp_buffer[22]);

        cali.dig_H1 = h1_buffer[0];
        cali.dig_H2 = (int16_t)(h2h6_buffer[1] << 8 | h2h6_buffer[0]);
        cali.dig_H3 = h2h6_buffer[2];
        cali.dig_H4 = (int16_t)(h2h6_buffer[3] << 4 | (h2h6_buffer[4] & 0x0F));
        cali.dig_H5 = (int16_t)(h2h6_buffer[5] << 4 | h2h6_buffer[4] >> 4);
        cali.dig_H6 = (int8_t)h2h6_buffer[6];
    }

    void BME280::read_all() {
        uint8_t buffer[8];
        if (read(BME280_PRESS_MSB, buffer, 8) != ESP_OK) {
            return;
        }

        // The first three registers are for Pressure (20 bits total).
        // Register 1 = Most Significant byte (use all 8 bits)
        // Register 2 = Least Significant byte (use all 8 bits)
        // Register 3 = Extra Least Significant bits (use only left 4 bits)
        uint32_t raw_press = (buffer[0] << 12) | buffer[1] << 4 | buffer[2] >> 4;

        // The next three registers are for Temperature (20 bits total).
        // Register 1 = Most Significant byte (use all 8 bits)
        // Register 2 = Least Significant byte (use all 8 bits)
        // Register 3 = Extra Least Significant bits (use only left 4 bits)
        uint32_t raw_temp = (buffer[3] << 12) | buffer[4] << 4 | buffer[5] >> 4;

        // The last two registers are for Humidity (16 bits total).
        // Register 1 = Most Significant byte (use all 8 bits)
        // Register 2 = Least Significant byte (use all 8 bits)
        uint32_t raw_hum = (buffer[6] << 8) | buffer[7];

        // Compensation formulas from BME280 datasheet
        int32_t var1 =
            ((((raw_temp >> 3) - ((int32_t)cali.dig_T1 << 1))) * ((int32_t)cali.dig_T2)) >> 11;
        int32_t var2 = (((((raw_temp >> 4) - ((int32_t)cali.dig_T1)) *
                          ((raw_temp >> 4) - ((int32_t)cali.dig_T1))) >>
                         12) *
                        ((int32_t)cali.dig_T3)) >>
                       14;
        int32_t t_fine = var1 + var2;

        std::lock_guard lock(dataMutex);
        temperature = (float)((t_fine * 5 + 128) >> 8) / 100.0;
        ESP_LOGI(BME_TAG, "Temperature : %f", temperature);
    }

    void BME280::read_humidity() {
        // ESP_LOGV(BME_TAG, "Chip Temperature: %d", temperature);
    }

    void BME280::read_pressure() {
        // ESP_LOGV(BME_TAG, "Chip Temperature: %d", temperature);
    }

    void BME280::read_temperature() {
        // ESP_LOGV(BME_TAG, "AccX: %d, AccY: %d, AccZ: %d", acc.x, acc.y, acc.z);
    }

    uint32_t BME280::getHumidity() const {
        std::lock_guard lock(dataMutex);
        return humidity;
    }

    uint32_t BME280::getPressure() const {
        std::lock_guard lock(dataMutex);
        return pressure;
    }

    int32_t BME280::getTemperature() const {
        std::lock_guard lock(dataMutex);
        return temperature;
    }
}  // namespace sensors
