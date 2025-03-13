#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "display.h"
#include "heltec_constants.h"

extern "C" void app_main() {
    initArduino();
    auto display = initDisplay();
    display.init();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "gay ahh clint");
    display.display();
}
