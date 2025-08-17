#include "sensors/bno055.h"

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <mutex>

/// @brief Control registers
#define BNO055_CHIP_ID_ADDR 0x00
#define BNO055_OPR_MODE_ADDR 0x3D
#define BNO055_PWR_MODE_ADDR 0x3E
#define BNO055_SYS_TRIGGER_ADDR 0x3F
#define BNO055_PAGE_ID_ADDR 0x07

/// @brief Data register starting points
#define BNO055_EULER_H_LSB 0x1A  // Euler Heading, Roll, Pitch (6 bytes, 2 per)
#define BNO055_MAG_X_LSB 0x0E    // Magnetometer X, Y, Z (6 bytes, 2 per)
#define BNO055_GYRO_X_LSB 0x14   // Gyroscope X, Y, Z (6 bytes, 2 per)
#define BNO055_ACC_X_LSB 0x08    // Accelerometer X, Y, Z (6 bytes, 2 per)
#define BNO055_CHIP_TEMP 0x34    // Chip Temperature (1 byte)

/// @brief Chip ID
#define BNO055_ID 0xA0

/// @brief Operating modes
#define OPERATION_MODE_NDOF 0x0C

namespace sensors {
    BNO055::BNO055(i2c::I2C_Config cfg, i2c_master_bus_handle_t bus_handle)
        : i2c::I2C_Interface(cfg, bus_handle) {
        init();

        // Start the sensor
        start_sensor(this);
    }

    void BNO055::run() {
        // Collect latest Euler readings
        read_euler();
        // Collect latest Magnetometer readings
        read_mag();
        // Collect latest Accelerometer readings
        read_acc();
        // Collect latest Gyroscope readings
        read_gyro();
        // Collect latest Chip Temperature readings
        read_temp();
    }

    esp_err_t BNO055::init() {
        uint8_t id = 0;
        ESP_ERROR_CHECK(read(BNO055_CHIP_ID_ADDR, &id, 1));
        if (id != BNO055_ID) {
            ESP_LOGE(name.c_str(), "Unexpected chip ID: 0x%02X", id);
            return ESP_FAIL;
        }

        // Set to config mode
        ESP_ERROR_CHECK(write(BNO055_OPR_MODE_ADDR, 0x00));
        vTaskDelay(pdMS_TO_TICKS(25));

        // Set power mode
        ESP_ERROR_CHECK(write(BNO055_PWR_MODE_ADDR, 0x00));
        vTaskDelay(pdMS_TO_TICKS(25));

        // Use page 0
        ESP_ERROR_CHECK(write(BNO055_PAGE_ID_ADDR, 0x00));
        vTaskDelay(pdMS_TO_TICKS(25));

        // Clear reset flag
        ESP_ERROR_CHECK(write(BNO055_SYS_TRIGGER_ADDR, 0x00));
        vTaskDelay(pdMS_TO_TICKS(25));

        // Set to NDOF mode now that we're done configuring
        ESP_ERROR_CHECK(write(BNO055_OPR_MODE_ADDR, OPERATION_MODE_NDOF));
        vTaskDelay(pdMS_TO_TICKS(25));

        return ESP_OK;
    }

    void BNO055::read_euler() {
        uint8_t buffer[6];
        if (read(BNO055_EULER_H_LSB, buffer, 6) != ESP_OK) {
            return;
        }

        // Combine each Least/Most-Significant-Byte into one value
        int16_t heading = static_cast<int16_t>(buffer[1]) << 8 | buffer[0];
        int16_t roll = static_cast<int16_t>(buffer[3]) << 8 | buffer[2];
        int16_t pitch = static_cast<int16_t>(buffer[5]) << 8 | buffer[4];

        std::lock_guard lock(dataMutex);
        euler.heading = heading / 16.0f;
        euler.roll = roll / 16.0f;
        euler.pitch = pitch / 16.0f;

        ESP_LOGV(name.c_str(), "Heading: %.2f, Roll: %.2f, Pitch: %.2f", euler.heading, euler.roll,
                 euler.pitch);
    }

    void BNO055::read_mag() {
        uint8_t buffer[6];
        if (read(BNO055_MAG_X_LSB, buffer, 6) != ESP_OK) {
            return;
        }

        // Combine each Least/Most-Significant-Byte into one value
        int16_t x = static_cast<int16_t>(buffer[1]) << 8 | buffer[0];
        int16_t y = static_cast<int16_t>(buffer[3]) << 8 | buffer[2];
        int16_t z = static_cast<int16_t>(buffer[5]) << 8 | buffer[4];

        std::lock_guard lock(dataMutex);
        mag.x = x;
        mag.y = y;
        mag.z = z;

        ESP_LOGV(name.c_str(), "MagX: %d, MagY: %d, MagZ: %d", mag.x, mag.y, mag.z);
    }

    void BNO055::read_acc() {
        uint8_t buffer[6];
        if (read(BNO055_ACC_X_LSB, buffer, 6) != ESP_OK) {
            return;
        }

        // Combine each Least/Most-Significant-Byte into one value
        int16_t x = static_cast<int16_t>(buffer[1]) << 8 | buffer[0];
        int16_t y = static_cast<int16_t>(buffer[3]) << 8 | buffer[2];
        int16_t z = static_cast<int16_t>(buffer[5]) << 8 | buffer[4];

        std::lock_guard lock(dataMutex);
        acc.x = x;
        acc.y = y;
        acc.z = z;

        ESP_LOGV(name.c_str(), "AccX: %d, AccY: %d, AccZ: %d", acc.x, acc.y, acc.z);
    }

    void BNO055::read_gyro() {
        uint8_t buffer[6];
        if (read(BNO055_GYRO_X_LSB, buffer, 6) != ESP_OK) {
            return;
        }

        // Combine each Least/Most-Significant-Byte into one value
        int16_t x = static_cast<int16_t>(buffer[1]) << 8 | buffer[0];
        int16_t y = static_cast<int16_t>(buffer[3]) << 8 | buffer[2];
        int16_t z = static_cast<int16_t>(buffer[5]) << 8 | buffer[4];

        std::lock_guard lock(dataMutex);
        gyro.x = x;
        gyro.y = y;
        gyro.z = z;

        ESP_LOGV(name.c_str(), "GyroX: %d, GyroY: %d, GyroZ: %d", gyro.x, gyro.y, gyro.z);
    }

    void BNO055::read_temp() {
        uint8_t buffer[1];
        if (read(BNO055_CHIP_TEMP, buffer, 1) != ESP_OK) {
            return;
        }

        std::lock_guard lock(dataMutex);
        temperature = buffer[0];

        ESP_LOGV(name.c_str(), "Chip Temperature: %d", temperature);
    }

    bno055_euler BNO055::getEuler() const {
        std::lock_guard lock(dataMutex);
        return euler;
    }

    bno055_vec3 BNO055::getMag() const {
        std::lock_guard lock(dataMutex);
        return mag;
    }

    bno055_vec3 BNO055::getAcc() const {
        std::lock_guard lock(dataMutex);
        return acc;
    }

    bno055_vec3 BNO055::getGyro() const {
        std::lock_guard lock(dataMutex);
        return gyro;
    }

    int8_t BNO055::getTemp() const {
        std::lock_guard lock(dataMutex);
        return temperature;
    }
}  // namespace sensors
