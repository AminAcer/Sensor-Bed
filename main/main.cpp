#include <Arduino.h>
#include <pins.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "display.h"

extern "C" void app_main() {
    SSD1306Wire display(0x3C, HELTEC_SDA_OLED, HELTEC_SCL_OLED);  // Adjust based on your wiring
    initArduino();
    initOLED();
    display.init();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "gay ahh clint");
    display.display();
}
