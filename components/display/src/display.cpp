#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "soc/gpio_num.h"
#include "display.h"

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
