#pragma once

#include <driver/i2c_types.h>
#include <soc/gpio_num.h>

#include <cstdint>

#include "protocols/i2c_interface.h"

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
        /// @brief Constructor
        ///
        /// @param cfg: I2C Config
        /// @param bus_handle: I2C Bus Handle to add device handle to
        BME280(i2c::I2C_Config cfg, i2c_master_bus_handle_t bus_handle);
        ~BME280() = default;

        // === Rule Of 5 ===
        BME280(const BME280&) = delete;             // Copy constructor
        BME280& operator=(const BME280&) = delete;  // Copy assignment
        BME280(BME280&&) = delete;                  // Move constructor
        BME280& operator=(BME280&&) = delete;       // Move assignment

        /// @brief Method to retrieve latest Humidity readings
        ///
        /// @return uint32_t: Latest Humidity readings
        [[nodiscard]] uint32_t getHumidity() const;

        /// @brief Method to retrieve latest Pressure readings
        ///
        /// @return double: Latest Pressure readings
        [[nodiscard]] double getPressure() const;

        /// @brief Method to retrieve latest Temperature readings
        ///
        /// @return float: Latest Temperature readings
        [[nodiscard]] float getTemperature() const;

        /// @brief I2C Interface function that runs the sensor
        void run() final;

       private:
        /// @brief I2C Interface function that initializes the sensor
        esp_err_t init() final;

        /// @brief Method to read/store Calibration data from sensor
        void read_calibration();

        /// @brief Method to read/store Humidity, Pressure, and Temperature data from sensor
        void read_all();

        /// @brief Method to find the T_Fine value through compensation formulas
        ///
        /// @param raw_temp: Raw temperature
        ///
        /// @return int32_t: Calculated T_Fine parameter
        int32_t solve_tfine(uint32_t raw_temp);

        /// @brief Method to calculate Pressure using compensation formulas
        ///
        /// @param raw_pressure: Raw pressure from data line
        /// @param t_fine: Previously calculated T_Fine param
        ///
        /// @return double: Calculated Pressure
        double compensate_pressure(uint32_t raw_pressure, int32_t t_fine);

        /// @brief Method to calculate Humidity using compensation formulas
        ///
        /// @param raw_humidity: Raw humidity from data line
        /// @param t_fine: Previously calculated T_Fine param
        ///
        /// @return uint32_t: Calculated Humidity
        uint32_t compensate_humidity(uint32_t raw_humidity, int32_t t_fine);

        /// @brief Calibration readings from the sensor
        bme280_calibration_data cali;

        /// @brief Humidity readings from the sensor
        uint32_t humidity;

        /// @brief Pressure readings from the sensor
        double pressure;

        /// @brief Temperature readings from the sensor
        float temperature;
    };
}  // namespace sensors
