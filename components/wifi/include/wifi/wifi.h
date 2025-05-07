#pragma once

#include <SSD1306Wire.h>

namespace wifi {
    /// @brief Initializes the wifi
    ///
    /// @param display: Interface to control the OLED
    ///
    /// @return int: Result of the function (0: Success | 1: Failure)
    int init_wifi();
}  // namespace wifi
