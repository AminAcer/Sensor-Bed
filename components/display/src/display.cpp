#include "display.h"

#include "constants/heltec_pins.h"
#include "driver/gpio.h"
#include "freertos/task.h"

void turn_off_oled() {
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_36, 1);
}

void display_text(SSD1306Wire* display, const char* text, uint8_t posx, uint8_t posy) {
    display->clear();
    display->drawString(posx, posy, text);
    display->display();
}

void init_display_pins() {
    // Turning OLED on (without this, the OLED is very dim)
    gpio_set_direction(HELTEC_VEXT, GPIO_MODE_OUTPUT);
    gpio_set_level(HELTEC_VEXT, 0);

    // Resetting the OLED
    gpio_set_direction(HELTEC_RST_OLED, GPIO_MODE_OUTPUT);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(HELTEC_RST_OLED, 1);
}
