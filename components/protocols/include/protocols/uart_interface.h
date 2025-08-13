#pragma once

#include <driver/uart.h>
#include <esp_err.h>
#include <soc/gpio_num.h>

#include <cstdint>
#include <mutex>
#include <string>

namespace uart {
    struct UART_Config {
        /// Name for the sensor
        std::string name;
        /// UART port to use for this sensor
        uart_port_t uartPort;
        /// GPIO pin for the RX port
        gpio_num_t rxPin;
        /// GPIO pin for the TX port
        gpio_num_t txPin;
    };

    class UART_Interface {
       public:
        /// @brief Constructor
        ///
        /// @param cfg: UART Config
        explicit UART_Interface(const UART_Config& cfg);

        // === Rule Of 5 ===
        UART_Interface(const UART_Interface&) = delete;             // Copy constructor
        UART_Interface& operator=(const UART_Interface&) = delete;  // Copy assignment
        UART_Interface(UART_Interface&&) = delete;                  // Move constructor
        UART_Interface& operator=(UART_Interface&&) = delete;       // Move assignment

        /// @brief Sensor-specific run function that constantly gets data from the sensor
        virtual void run() = 0;

        /// @brief Name of the sensor
        std::string name;

       protected:
        /// @brief Mutex to guard data
        mutable std::mutex dataMutex;
    };

    /// @brief Starts the run/process loop of a sensor in a Task
    ///
    /// @param UART_interface: The UART Interface to start
    void start_sensor(UART_Interface* uart_interface);

    /// @brief Initializes the UART ports using the desired config
    ///
    /// @return int: Result of the function (ESP_OK: Success | OTHERS: Failure)
    esp_err_t init_all_uart_ports();
}  // namespace uart
