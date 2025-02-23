#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "esp_log.h"

void app_main(void)
{
  char* ourTaskName = pcTaskGetName(NULL);
  ESP_LOGI(ourTaskName, "Hello...\n");

  while(1)
  {
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }

}
