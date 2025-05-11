#include "i2c/i2c.h"

#include <driver/i2c_master.h>
#include <driver/i2c_types.h>

#include "logger/logger.h"

const static char* TAG = "I2C";

namespace i2c {
    void setup_bus(i2c_master_bus_handle_t* bus_handle, gpio_num_t sda, gpio_num_t scl,
                   uint16_t dev_addr) {
        i2c_master_bus_config_t i2c_master_config = {
            .i2c_port = I2C_NUM_1,
            .sda_io_num = sda,
            .scl_io_num = scl,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags = {.enable_internal_pullup = true, .allow_pd = false},
        };

        ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_config, bus_handle));

        i2c_master_dev_handle_t dev_handle;

        i2c_device_config_t dev_cfg = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = dev_addr,
            .scl_speed_hz = 100000,
        };

        ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_cfg, &dev_handle));

        D_LOGI(TAG, "I2C init successful");
    }
}  // namespace i2c
