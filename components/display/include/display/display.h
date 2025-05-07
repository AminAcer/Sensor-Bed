#pragma once

#include <SSD1306Wire.h>

#include <cstdint>

namespace display {
    enum FontSize { SMALL = 1, MEDIUM = 2, LARGE = 3 };

    /// @brief Initializes the display pins by resetting/setting them
    void init_display();

    /// @brief Displays text on the display after clearing it
    ///
    /// @param text: Text to display
    /// @param size: Size of the text
    /// @param posx: X position to display the text
    /// @param posy: Y position to display the text
    void display_text(const char* text, FontSize size = SMALL, uint8_t posx = 0, uint8_t posy = 0);

    /// @brief Turns the OLED off
    void turn_off_oled();
}  // namespace display
