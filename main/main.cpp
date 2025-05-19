#include <Arduino.h>
#include <Wire.h>

#include <cstdint>

#include "dis/dis.h"
#include "driver/uart.h"
#include "hal/uart_types.h"
#include "logger/logger.h"
#include "sensors/ada254.h"
#include "sensors/bme280.h"
#include "sensors/bno055.h"
#include "sockets/sockets.h"
#include "wifi/wifi.h"

static const char* TAG = "MAIN";

extern "C" void app_main() {
    initArduino();
    display::init_display();
    D_LOGI(TAG, "Display Initialized");

    wifi::init_wifi();

    static auto svr = sockets::udp::Socket(sockets::udp::SocketType::SERVER, "0.0.0.0", 46729);
    static auto client =
        sockets::udp::Socket(sockets::udp::SocketType::CLIENT, "192.168.0.183", 36729);

    svr.callback = sockets::udp::basic_handle;
    sockets::udp::start_receive(&svr);
    client.send("Client test msg");

    // Make an Entity
    DIS::EntityStatePdu ownship = create_entity(0.1, 0.3, 10.4);
    D_LOG_CORD(TAG, "ground", ownship.getEntityLocation());

    // Init I2C Master bus
    static i2c_master_bus_handle_t bus_handle =
        i2c::init_i2c_master_bus(GPIO_NUM_7, GPIO_NUM_6, I2C_NUM_1);

    // Init SPI Master bus
    ESP_ERROR_CHECK(spi::init_spi_master_bus(GPIO_NUM_42, GPIO_NUM_41, GPIO_NUM_45, SPI3_HOST));

    static auto ada254 = sensors::ADA254(
        spi::SPI_Config{.name = "ADA254", .cs = GPIO_NUM_40, .speed_hz = (uint16_t)50000});

    static auto bno055 = sensors::BNO055(
        i2c::I2C_Config{.name = "BNO055", .dev_addr = 0x28, .scl_speed_hz = (uint16_t)100000},
        bus_handle);

    static auto bme280 = sensors::BME280(
        i2c::I2C_Config{.name = "BME280", .dev_addr = 0x76, .scl_speed_hz = (uint16_t)100000},
        bus_handle);

    static auto bme280_2 = sensors::BME280(
        i2c::I2C_Config{.name = "BME280-2", .dev_addr = 0x77, .scl_speed_hz = (uint16_t)100000},
        bus_handle);

    uart_config_t gps1_cfg = {.baud_rate = 9600,
                              .data_bits = UART_DATA_8_BITS,
                              .parity = UART_PARITY_DISABLE,
                              .stop_bits = UART_STOP_BITS_1,
                              .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_0, &gps1_cfg);
    uart_set_pin(UART_NUM_0, GPIO_NUM_5, GPIO_NUM_4, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 4096 * 2, 0, 0, NULL, 0);

    uint8_t* gps1_data = (uint8_t*)malloc(4096);
    while (true) {
        ESP_LOGI(TAG, "INSIDE");
        int len = uart_read_bytes(UART_NUM_0, gps1_data, 4096 - 1, pdMS_TO_TICKS(50));
        if (len > 0) {
            gps1_data[len] = '\0';
            D_LOGI(TAG, "GPS data: %s", (char*)gps1_data);
        } else {
        }
    }

    // Write latest data to the SD Card at 20hz
    while (true) {
        ada254.write(sensors::FileName::ORIENT, "ORIENTATION %f\n", bno055.getEuler().pitch);
        ada254.write(sensors::FileName::TEMP, "TEMPERATURE %f\n", bme280.getTemperature());
        ada254.write(sensors::FileName::PRESSURE, "PRESSURE %f\n", bme280.getPressure());
        ada254.write(sensors::FileName::HUMIDITY, "HUMIDITY %d\n", bme280.getHumidity());
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
