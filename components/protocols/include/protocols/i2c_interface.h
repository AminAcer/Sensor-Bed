#pragma once

#include <driver/i2c_types.h>
#include <esp_err.h>
#include <soc/gpio_num.h>

#include <cstdint>
#include <mutex>
#include <string>

namespace i2c {
    struct I2C_Config {
        /// Name for the sensor
        std::string name;
        /// Device address
        uint16_t dev_addr;
        /// Rate of the SCL line
        uint16_t scl_speed_hz;
    };

    class I2C_Interface {
       public:
        /// @brief Constructor
        ///
        /// @param cfg: I2C Config
        /// @param bus_handle: I2C Bus Handle to add device handle to
        I2C_Interface(I2C_Config cfg, i2c_master_bus_handle_t bus_handle);
        ~I2C_Interface() = default;

        // === Rule Of 5 ===
        I2C_Interface(const I2C_Interface&) = delete;             // Copy constructor
        I2C_Interface& operator=(const I2C_Interface&) = delete;  // Copy assignment
        I2C_Interface(I2C_Interface&&) = delete;                  // Move constructor
        I2C_Interface& operator=(I2C_Interface&&) = delete;       // Move assignment

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
        virtual esp_err_t write(uint8_t reg, uint8_t data);

        /// @brief Read function that reads data from a sensors registers
        ///
        /// @param reg: Register address to write to
        /// @param data: Packet to populate with the data from the register
        /// @param len: Size of the packet
        ///
        /// @return int: Result of the function (ESP_OK: Success | OTHERS: Failure)
        virtual esp_err_t read(uint8_t reg, uint8_t* data, size_t len);

        /// @brief Device Handle
        i2c_master_dev_handle_t dev_handle;

        /// @brief Mutex to guard data
        mutable std::mutex dataMutex;
    };

    /// @brief Starts the run/process loop of a sensor in a Task
    ///
    /// @param i2c_interface: The I2C Interface to start
    void start_sensor(I2C_Interface* i2c_interface);

    /// @brief Initializes the I2C Master bus on the requested pins
    ///
    /// @param sda_port: The pin number for the SDA line
    /// @param scl_port: The pin number for the SCL line
    /// @param i2c_port: The i2c port (either NUM_0 or NUM_1)
    ///
    /// @return i2c_master_bus_handle_t: Configured master bus handle
    i2c_master_bus_handle_t init_i2c_master_bus(gpio_num_t sda_port, gpio_num_t scl_port,
                                                i2c_port_num_t i2c_port);
}  // namespace i2c
