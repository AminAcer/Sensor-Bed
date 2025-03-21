#pragma once

#include <cstdint>

#include "SSD1306Wire.h"

/// @brief Initializes the display pins by resetting/setting them
void init_display_pins();

/// @brief Displays text on the display after clearing it
void display_text(SSD1306Wire* display, const char* text, uint8_t posx = 0, uint8_t posy = 0);

/// @brief Turns the OLED off
void turn_off_oled();
