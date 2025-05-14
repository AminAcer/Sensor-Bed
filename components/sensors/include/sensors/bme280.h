#pragma once

#include <driver/i2c_types.h>
#include <esp_err.h>
#include <soc/gpio_num.h>

#include <cstdint>
#include <mutex>

#include "i2c/I2C_Interface.h"

namespace sensors {
    struct bme280_calibration_data {
        // Temperature
        uint16_t dig_T1;
        int16_t dig_T2;
        int16_t dig_T3;

        // Pressure
        uint16_t dig_P1;
        int16_t dig_P2;
        int16_t dig_P3;
        int16_t dig_P4;
        int16_t dig_P5;
        int16_t dig_P6;
        int16_t dig_P7;
        int16_t dig_P8;
        int16_t dig_P9;

        // Humidity
        uint8_t dig_H1;
        int16_t dig_H2;
        uint8_t dig_H3;
        int16_t dig_H4;
        int16_t dig_H5;
        int8_t dig_H6;
    };

    class BME280 : public i2c::I2C_Interface {
       public:
        /// @brief Constructor for a UDP socket
        ///
        /// @param cfg: I2C Config
        explicit BME280(i2c::I2C_Config cfg);
        ~BME280() = default;

        // === Rule Of 5 ===
        BME280(const BME280&) = delete;                 // Copy constructor
        BME280& operator=(const BME280&) = delete;      // Copy assignment
        BME280(BME280&&) noexcept = delete;             // Move constructor
        BME280& operator=(BME280&&) noexcept = delete;  // Move assignment

        /// @brief Method to retrieve latest Humidity readings
        ///
        /// @return uint32_t: Latest Humidity readings
        [[nodiscard]] uint32_t getHumidity() const;

        /// @brief Method to retrieve latest Pressure readings
        ///
        /// @return uint32_t: Latest Pressure readings
        [[nodiscard]] uint32_t getPressure() const;

        /// @brief Method to retrieve latest Temperature readings
        ///
        /// @return int32_t: Latest Temperature readings
        [[nodiscard]] int32_t getTemperature() const;

        /// @brief I2C Interface function that runs the sensor
        virtual void run() override;

       private:
        /// @brief I2C Interface function that initializes the sensor
        virtual esp_err_t init() override;

        /// @brief Method to read/store Calibration data from sensor
        void read_calibration();

        /// @brief Method to read/store Humidity, Pressure, and Temperature data from sensor
        void read_all();

        /// @brief Method to read/process Humidity data from sensor
        void read_humidity();

        /// @brief Method to read/process Pressure data from sensor
        void read_pressure();

        /// @brief Method to read/process Temperature data from sensor
        void read_temperature();

        /// @brief Calibration readings from the sensor
        bme280_calibration_data cali;

        /// @brief Humidity readings from the sensor
        uint32_t humidity;

        /// @brief Pressure readings from the sensor
        uint32_t pressure;

        /// @brief Temperature readings from the sensor
        int32_t temperature;

        /// @brief Mutex to guard data
        mutable std::mutex dataMutex;
    };
}  // namespace sensors
