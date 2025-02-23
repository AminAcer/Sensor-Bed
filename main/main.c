#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "esp_log.h"
#include "driver/gpio.h"

#define WHITE_LED 35

void app_main(void)
{
  char* ourTaskName = pcTaskGetName(NULL);
  ESP_LOGI(ourTaskName, "Hello...\n");

  gpio_reset_pin(WHITE_LED);
  gpio_set_direction(WHITE_LED, GPIO_MODE_OUTPUT);

  while(1)
  {
    gpio_set_level(WHITE_LED, 0);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    gpio_set_level(WHITE_LED, 1);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }

}
