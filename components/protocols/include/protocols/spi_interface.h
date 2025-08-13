#pragma once

#include <driver/spi_master.h>
#include <soc/gpio_num.h>

#include <cstdint>
#include <mutex>
#include <string>

namespace spi {
    struct SPI_Config {
        /// Name for the sensor/chip
        std::string name;
        /// Chip Select Pin
        gpio_num_t cs;
        /// Clock Speed in Hz
        uint16_t speed_hz;
    };

    class SPI_Interface {
       public:
        /// @brief Constructor
        ///
        /// @param cfg: SPI Config
        explicit SPI_Interface(SPI_Config cfg);
        ~SPI_Interface() = default;

        // === Rule Of 5 ===
        SPI_Interface(const SPI_Interface&) = delete;             // Copy constructor
        SPI_Interface& operator=(const SPI_Interface&) = delete;  // Copy assignment
        SPI_Interface(SPI_Interface&&) = delete;                  // Move constructor
        SPI_Interface& operator=(SPI_Interface&&) = delete;       // Move assignment

        /// @brief Name of the sensor
        std::string name;

       protected:
        /// @brief Sensor-specific init function that initializes the sensor
        ///
        /// @return int: Result of the function (ESP_OK: Success | OTHERS: Failure)
        virtual esp_err_t init() = 0;

        /// @brief Device Handle
        spi_device_handle_t dev_handle;

        /// @brief Mutex to guard data
        mutable std::mutex dataMutex;
    };

    /// @brief Initializes the SPI Master bus on the requested pins
    ///
    /// @param miso_port: The pin number for the MISO line
    /// @param mosi_port: The pin number for the MOSI line
    /// @param clk_port: The pin number for the Clock line
    /// @param spi_host: The SPI Host
    ///
    /// @return int: Result of the function (ESP_OK: Success | OTHERS: Failure)
    esp_err_t init_spi_master_bus(gpio_num_t miso_port, gpio_num_t mosi_port, gpio_num_t clk_port,
                                  spi_host_device_t spi_host);
}  // namespace spi
