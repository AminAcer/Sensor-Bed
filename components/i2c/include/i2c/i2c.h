#pragma once

#include <soc/gpio_num.h>

#include <cstdint>

#include "driver/i2c_types.h"

namespace i2c {
    /// @brief Initializes a i2c bus
    ///
    /// @param bus_handle: Master bus handle
    /// @param sda: Port number for the SDA line
    /// @param scl: Port number for the SCL line
    ///
    /// @return int: Result of the function (0: Success | 1: Failure)
    void setup_bus(i2c_master_bus_handle_t* bus_handle, gpio_num_t sda, gpio_num_t scl,
                   uint16_t dev_addr);
}  // namespace i2c
