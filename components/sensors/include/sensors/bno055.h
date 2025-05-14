#pragma once

#include <driver/i2c_types.h>
#include <esp_err.h>
#include <soc/gpio_num.h>

#include <cstdint>
#include <mutex>

#include "i2c/I2C_Interface.h"

namespace sensors {
    struct bno055_euler {
        float heading;
        float roll;
        float pitch;
    };

    struct bno055_vec3 {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    class BNO055 : public i2c::I2C_Interface {
       public:
        /// @brief Constructor for a UDP socket
        ///
        /// @param cfg: I2C Config
        /// @param bus_handle: I2C Bus Handle to add device handle to
        BNO055(i2c::I2C_Config cfg, i2c_master_bus_handle_t bus_handle);
        ~BNO055() = default;

        // === Rule Of 5 ===
        BNO055(const BNO055&) = delete;                 // Copy constructor
        BNO055& operator=(const BNO055&) = delete;      // Copy assignment
        BNO055(BNO055&&) noexcept = delete;             // Move constructor
        BNO055& operator=(BNO055&&) noexcept = delete;  // Move assignment

        /// @brief Method to retrieve latest Euler readings
        ///
        /// @return bno055_euler: Latest Euler readings
        [[nodiscard]] bno055_euler getEuler() const;

        /// @brief Method to retrieve latest Magnetometer readings
        ///
        /// @return bno055_vec3: Latest Magnetometer readings
        [[nodiscard]] bno055_vec3 getMag() const;

        /// @brief Method to retrieve latest Accelerometer readings
        ///
        /// @return bno055_vec3: Latest Accelerometer readings
        [[nodiscard]] bno055_vec3 getAcc() const;

        /// @brief Method to retrieve latest Gyroscope readings
        ///
        /// @return bno055_vec3: Latest Gyroscope readings
        [[nodiscard]] bno055_vec3 getGyro() const;

        /// @brief Method to retrieve latest Chip Temperature readings
        ///
        /// @return int8_t: Latest Chip Temperature readings
        [[nodiscard]] int8_t getTemp() const;

        /// @brief I2C Interface function that runs the sensor
        virtual void run() override;

       private:
        /// @brief I2C Interface function that initializes the sensor
        virtual esp_err_t init() override;

        /// @brief Method to read/process Euler data from sensor
        void read_euler();

        /// @brief Method to read/process Magnetometer data from sensor
        void read_mag();

        /// @brief Method to read/process Accelerometer data from sensor
        void read_acc();

        /// @brief Method to read/process Gyroscope data from sensor
        void read_gyro();

        /// @brief Method to read/process the Temperature of the sensor itself
        void read_temp();

        /// @brief Euler readings from the sensor (degrees)
        bno055_euler euler;

        /// @brief Magnetometer readings from the sensor (uT)
        bno055_vec3 mag;

        /// @brief Acceleration readings from the sensor (m/s^2)
        bno055_vec3 acc;

        /// @brief Gyroscope readings from the sensor (deg/sec)
        bno055_vec3 gyro;

        /// @brief Temperature of the sensor chip itself (celsius)
        int8_t temperature;

        /// @brief Mutex to guard data
        mutable std::mutex dataMutex;
    };
}  // namespace sensors
