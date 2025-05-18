#include <Arduino.h>
#include <Wire.h>

#include <cstdint>

#include "dis/dis.h"
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
    ESP_ERROR_CHECK(spi::init_spi_master_bus(GPIO_NUM_41, GPIO_NUM_40, GPIO_NUM_42, SPI3_HOST));

    static auto ada254 = sensors::ADA254(
        spi::SPI_Config{.name = "ADA254", .cs = GPIO_NUM_39, .speed_hz = (uint16_t)50000});

    static auto bno055 = sensors::BNO055(
        i2c::I2C_Config{.name = "BNO055", .dev_addr = 0x28, .scl_speed_hz = (uint16_t)100000},
        bus_handle);

    static auto bme280 = sensors::BME280(
        i2c::I2C_Config{.name = "BME280", .dev_addr = 0x76, .scl_speed_hz = (uint16_t)100000},
        bus_handle);

    static auto bme280_2 = sensors::BME280(
        i2c::I2C_Config{.name = "BME280-2", .dev_addr = 0x77, .scl_speed_hz = (uint16_t)100000},
        bus_handle);

    // Write latest data to the SD Card at 20hz
    while (true) {
        ada254.write(sensors::FileName::ORIENT, "ORIENTATION %f\n", bno055.getEuler().pitch);
        ada254.write(sensors::FileName::TEMP, "TEMPERATURE %f\n", bme280.getTemperature());
        ada254.write(sensors::FileName::PRESSURE, "PRESSURE %f\n", bme280.getPressure());
        ada254.write(sensors::FileName::HUMIDITY, "HUMIDITY %d\n", bme280.getHumidity());
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
