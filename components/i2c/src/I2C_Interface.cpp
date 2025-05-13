#include "i2c/I2C_Interface.h"

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>

#include "esp_err.h"
#include "logger/logger.h"

const static char* TAG = "I2C";
namespace i2c {
    I2C_Interface::I2C_Interface(I2C_Config cfg) : name(cfg.name) {
        i2c_master_bus_config_t i2c_master_config = {
            .i2c_port = I2C_NUM_1,
            .sda_io_num = cfg.sda,
            .scl_io_num = cfg.scl,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags = {.enable_internal_pullup = true, .allow_pd = false},
        };

        if (i2c_new_master_bus(&i2c_master_config, &bus_handle) != ESP_OK) {
            D_LOGW(TAG, "I2C failed to initialize");
        }

        i2c_device_config_t dev_cfg = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = cfg.dev_addr,
            .scl_speed_hz = cfg.scl_speed_hz,
        };

        if (i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle) != ESP_OK) {
            D_LOGW(TAG, "I2C failed to add device");
        }

        D_LOGI(TAG, "I2C init successful");
    }

    esp_err_t I2C_Interface::write(uint8_t reg, uint8_t data) {
        uint8_t buffer[2] = {reg, data};
        return i2c_master_transmit(dev_handle, buffer, sizeof(buffer), 1000);  // 1 second timeout
    }

    esp_err_t I2C_Interface::read(uint8_t reg, uint8_t* data, size_t len) {
        // First send the register address to read from
        if (esp_err_t res = i2c_master_transmit(dev_handle, &reg, len, 1000); res != ESP_OK) {
            return res;
        }

        // Then receive the requested number of bytes
        return i2c_master_receive(dev_handle, data, len, 1000);
    }

    static void process_sensor(void* arg) {
        I2C_Interface* sensor = (I2C_Interface*)arg;
        while (true) {
            sensor->run();
            // Delay processing
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Cleanup if loop exists
        vTaskDelete(NULL);
    }

    void start_sensor(I2C_Interface* sensor) {
        if (sensor == nullptr) {
            return;
        }

        const char* task_name = sensor->name.c_str();
        xTaskCreatePinnedToCore(process_sensor, task_name,
                                4096,           // Allocate 16KB to the stack of this task
                                (void*)sensor,  // Pass the sensor argument in
                                6, NULL, 1);
    }
}  // namespace i2c
