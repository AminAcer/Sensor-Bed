#pragma once

#include <driver/i2c_types.h>
#include <esp_err.h>
#include <soc/gpio_num.h>

#include <cstdint>
#include <string>

namespace i2c {
    struct I2C_Config {
        /// @brief Name for the sensor
        std::string name;
        /// @brief Port number for the SDA line
        gpio_num_t sda;
        /// @brief Port number for the SCL line
        gpio_num_t scl;
        /// @brief Device address
        uint16_t dev_addr;
        /// @brief Rate of the SCL line
        uint16_t scl_speed_hz;
    };

    class I2C_Interface {
       public:
        /// @brief Constructor
        ///
        /// @param cfg: I2C Config
        explicit I2C_Interface(I2C_Config cfg);
        ~I2C_Interface() = default;

        // === Rule Of 5 ===
        I2C_Interface(const I2C_Interface&) = default;                 // Copy constructor
        I2C_Interface& operator=(const I2C_Interface&) = default;      // Copy assignment
        I2C_Interface(I2C_Interface&&) noexcept = default;             // Move constructor
        I2C_Interface& operator=(I2C_Interface&&) noexcept = default;  // Move assignment

        /// @brief Sensor-specific run function that constantly gets data from the sensor
        virtual void run() = 0;

        /// @brief Name of the sensor
        std::string name;

       protected:
        /// @brief Sensor-specific init function that initializes the sensor
        ///
        /// @return int: Result of the function (ESP_OK: Success | OTHERS: Failure)
        virtual esp_err_t init() = 0;

        /// @brief Write function that writes to a sensors register
        ///
        /// @param reg: Register address to write to
        /// @param data: Packet to send to the register
        ///
        /// @return int: Result of the function (ESP_OK: Success | OTHERS: Failure)
        esp_err_t write(uint8_t reg, uint8_t data);

        /// @brief Read function that reads data from a sensors registers
        ///
        /// @param reg: Register address to write to
        /// @param data: Packet to populate with the data from the register
        /// @param len: Size of the packet
        ///
        /// @return int: Result of the function (ESP_OK: Success | OTHERS: Failure)
        esp_err_t read(uint8_t reg, uint8_t* data, size_t len);

        /// @brief Bus Handle
        i2c_master_bus_handle_t bus_handle;

        /// @brief Device Handle
        i2c_master_dev_handle_t dev_handle;
    };

    /// @brief Starts the run/process loop of a sensor in a Task
    ///
    /// @param i2c_interface: The I2C Interface to start
    void start_sensor(I2C_Interface* i2c_interface);
}  // namespace i2c
