
#include "driver/uart.h"
#include "esp_err.h"
#include "hal/uart_types.h"
#include "logger/logger.h"
#include "protocols/uart_interface.h"

#define UART_BAUD_RATE 9600
#define UART_RX_BUFFER_SIZE 2048
#define UART_TX_BUFFER_SIZE 0  // TX is currently unused

namespace uart {
    const static char* TAG = "UART";

    UART_Interface::UART_Interface(const UART_Config& cfg) : name(cfg.name) {
        uart_set_pin(cfg.uartPort, cfg.rxPin, cfg.txPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    }

    static void process_sensor(void* arg) {
        UART_Interface* sensor = (UART_Interface*)arg;
        while (true) {
            sensor->run();
            // Delay processing
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Cleanup if loop exists
        vTaskDelete(NULL);
    }

    void start_sensor(UART_Interface* sensor) {
        if (sensor == nullptr) {
            return;
        }

        const char* task_name = sensor->name.c_str();
        xTaskCreatePinnedToCore(process_sensor, task_name,
                                4096,           // Allocate 4KB to the stack of this task
                                (void*)sensor,  // Pass the sensor argument in
                                6, NULL, 1);
    }

    esp_err_t init_all_uart_ports() {
        uart_config_t cfg = {
            .baud_rate = UART_BAUD_RATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_DEFAULT,
        };

        uart_param_config(UART_NUM_0, &cfg);
        uart_param_config(UART_NUM_1, &cfg);
        uart_param_config(UART_NUM_2, &cfg);
        uart_driver_install(UART_NUM_0, UART_RX_BUFFER_SIZE, UART_TX_BUFFER_SIZE, 0, NULL, 0);
        uart_driver_install(UART_NUM_1, UART_RX_BUFFER_SIZE, UART_TX_BUFFER_SIZE, 0, NULL, 0);
        uart_driver_install(UART_NUM_2, UART_RX_BUFFER_SIZE, UART_TX_BUFFER_SIZE, 0, NULL, 0);
        return ESP_OK;
    }
}  // namespace uart
