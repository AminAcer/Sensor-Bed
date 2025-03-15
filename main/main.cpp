#include <Arduino.h>
#include <Wire.h>

#include "display.h"
#include "wifi.h"

extern "C" void app_main() {
    initArduino();
    SSD1306Wire display = initDisplay();
    display.init();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "testing");
    display.display();
    init_wifi();
}
