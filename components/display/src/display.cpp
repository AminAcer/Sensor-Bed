#include "display/display.h"

#include <SSD1306Wire.h>
#include <driver/gpio.h>
#include <freertos/task.h>

#include "constants/heltec_pins.h"

void turn_off_oled() {
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_36, 1);
}

void display_text(SSD1306Wire* display, const char* text, font_size size, uint8_t posx,
                  uint8_t posy) {
    switch (size) {
        case (font_size::SMALL):
            display->setFont(ArialMT_Plain_10);
            break;
        case (font_size::MEDIUM):
            display->setFont(ArialMT_Plain_16);
            break;
        case (font_size::LARGE):
            display->setFont(ArialMT_Plain_24);
            break;
        default:
            display->setFont(ArialMT_Plain_16);
            break;
    };
    display->clear();
    display->drawString(posx, posy, text);
    display->display();
}

void init_display(SSD1306Wire* display) {
    // Turning OLED on (without this, the OLED is very dim)
    gpio_set_direction(HELTEC_VEXT, GPIO_MODE_OUTPUT);
    gpio_set_level(HELTEC_VEXT, 0);

    // Resetting the OLED
    gpio_set_direction(HELTEC_RST_OLED, GPIO_MODE_OUTPUT);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(HELTEC_RST_OLED, 1);

    // Init the display
    display->init();
}
