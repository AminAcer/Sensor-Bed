#include "display.h"

#include <stdio.h>

#include "constants/heltec_pins.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void turnOffOLED() {
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_36, 1);
}

SSD1306Wire initDisplay() {
    // Turning OLED on (without this, the OLED is very dim)
    gpio_set_direction(HELTEC_VEXT, GPIO_MODE_OUTPUT);
    gpio_set_level(HELTEC_VEXT, 0);

    // Resetting the OLED
    gpio_set_direction(HELTEC_RST_OLED, GPIO_MODE_OUTPUT);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(HELTEC_RST_OLED, 1);

    // Making the display object and returning it
    return SSD1306Wire(0x3C, HELTEC_SDA_OLED, HELTEC_SCL_OLED);
}
