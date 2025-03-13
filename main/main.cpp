#include <Arduino.h>
#include <pins.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "soc/gpio_num.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "display.h"

SSD1306Wire display(0x3C, VATS_SDA_OLED, VATS_SCL_OLED);  // Adjust based on your wiring

void turnOffOLED()
{
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_36, 1);
}

void initOLED()
{
    // Turning OLED on
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_36, 0);

    // Resetting the OLED
    gpio_set_direction(GPIO_NUM_21, GPIO_MODE_OUTPUT);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    gpio_set_level(GPIO_NUM_21, 1);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    gpio_set_level(GPIO_NUM_21, 0);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    gpio_set_level(GPIO_NUM_21, 1);    
}

extern "C" void app_main() {
    initArduino();
    initOLED();
    my_component_function();
    display.init();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "gay ahh clint");
    display.display();
}
