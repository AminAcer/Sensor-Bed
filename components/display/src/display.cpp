#include "display/display.h"

#include <SSD1306Wire.h>
#include <driver/gpio.h>
#include <freertos/task.h>

#include "constants/heltec_pins.h"

namespace display {
    // Display object for writing to the display
    static SSD1306Wire display = SSD1306Wire(0x3C, HELTEC_SDA_OLED, HELTEC_SCL_OLED);

    void turn_off_oled() {
        gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);
        gpio_set_level(GPIO_NUM_36, 1);
    }

    void display_text(const char* text, FontSize size, uint8_t posx, uint8_t posy) {
        switch (size) {
            case (FontSize::SMALL):
                display.setFont(ArialMT_Plain_10);
                break;
            case (FontSize::MEDIUM):
                display.setFont(ArialMT_Plain_16);
                break;
            case (FontSize::LARGE):
                display.setFont(ArialMT_Plain_24);
                break;
            default:
                display.setFont(ArialMT_Plain_16);
                break;
        };
        display.clear();
        display.drawString(posx, posy, text);
        display.display();
    }

    void display_cords(const char* entityId, const DIS::Vector3Double& pos) {
        display.clear();
        display.drawString(posx, posy, text);
        display.display();
    }

    void init_display() {
        // Turning OLED on (without this, the OLED is very dim)
        gpio_set_direction(HELTEC_VEXT, GPIO_MODE_OUTPUT);
        gpio_set_level(HELTEC_VEXT, 0);

        // Resetting the OLED
        gpio_set_direction(HELTEC_RST_OLED, GPIO_MODE_OUTPUT);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(HELTEC_RST_OLED, 1);

        // Init the display
        display.init();

        // Invert display
        display.flipScreenVertically();
    }
}  // namespace display
