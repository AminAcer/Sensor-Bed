#pragma once
#include "SSD1306Wire.h"

/// @brief Initializes the display
/// @return Constructed SSD1306 object (the display)
SSD1306Wire initDisplay();

/// @brief Turns the OLED off
void turnOffOLED();
