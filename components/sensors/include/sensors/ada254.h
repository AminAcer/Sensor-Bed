#pragma once

#include <driver/i2c_types.h>
#include <soc/gpio_num.h>

#include "protocols/spi_interface.h"
#include "sd_protocol_types.h"

namespace sensors {
    enum class FileName { ORIENT = 1, TEMP = 2, PRESSURE = 3, HUMIDITY = 4 };

    class ADA254 : public spi::SPI_Interface {
       public:
        /// @brief Constructor
        ///
        /// @param cfg: SPI Config
        explicit ADA254(spi::SPI_Config cfg);
        ~ADA254();

        // === Rule Of 5 ===
        ADA254(const ADA254&) = delete;             // Copy constructor
        ADA254& operator=(const ADA254&) = delete;  // Copy assignment
        ADA254(ADA254&&) = delete;                  // Move constructor
        ADA254& operator=(ADA254&&) = delete;       // Move assignment

        /// @brief Write to the SD card
        ///
        /// @param file: File enum to write data to
        /// @param fmt: Data to write
        /// @param ...: Variadic args if needed for logging
        void write(FileName file, const char* fmt, ...);

       private:
        /// @brief SPI Interface function that initializes the sensor
        esp_err_t init() final;

        /// @brief Pointer to the sd card to write to
        sdmmc_card_t* card;

        /// @brief File pointer for EULER data
        FILE* orient{nullptr};

        /// @brief File pointer for TEMP data
        FILE* temp{nullptr};

        /// @brief File pointer for PRESSURE data
        FILE* pressure{nullptr};

        /// @brief File pointer for HUMIDITY data
        FILE* humidity{nullptr};
    };
}  // namespace sensors
