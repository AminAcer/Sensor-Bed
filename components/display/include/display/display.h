#pragma once

#include <cstdint>

#include "SSD1306Wire.h"

enum font_size { SMALL = 1, MEDIUM = 2, LARGE = 3 };

/// @brief Initializes the display pins by resetting/setting them
void init_display(SSD1306Wire* display);

/// @brief Displays text on the display after clearing it
void display_text(SSD1306Wire* display, const char* text, font_size size = SMALL, uint8_t posx = 0,
                  uint8_t posy = 0);

/// @brief Turns the OLED off
void turn_off_oled();
