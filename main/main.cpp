#include <Arduino.h>
#include <Wire.h>

#include <cstdint>

#include "dis/dis.h"
#include "logger/logger.h"
#include "protocols/uart_interface.h"
#include "sensors/ada254.h"
#include "sensors/bme280.h"
#include "sensors/bno055.h"
#include "sockets/sockets.h"
#include "wifi/wifi.h"

static const char* TAG = "MAIN";

#define ORIENTATION_BUFFER_MAX 128
#define ORIENTATION_FORMAT "Orientation:\n|- pitch: %.2f\n|- roll: %.2f\n|- heading: %.2f"

void initRadio() {
    // Make server to receive data from
    static auto svr = sockets::udp::Socket(sockets::udp::SocketType::SERVER, "0.0.0.0", 36729);
    svr.callback = sockets::udp::basic_handle;
    sockets::udp::start_receive(&svr);
}

void initBench() {
    // Make client to send data to the radio
    static auto udpClient =
        sockets::udp::Socket(sockets::udp::SocketType::CLIENT, "192.168.0.189", 36729);

    // Init I2C Master bus
    static i2c_master_bus_handle_t bus_handle =
        i2c::init_i2c_master_bus(GPIO_NUM_7, GPIO_NUM_6, I2C_NUM_1);

    // Init SPI Master bus
    ESP_ERROR_CHECK(spi::init_spi_master_bus(GPIO_NUM_42, GPIO_NUM_41, GPIO_NUM_45, SPI3_HOST));

    // Init UART Master bus
    ESP_ERROR_CHECK(uart::init_all_uart_ports());

    // Init all sensors
    static auto sdcard = sensors::ADA254(spi::SPI_Config{
        .name = "ADA254", .cs = GPIO_NUM_40, .speed_hz = static_cast<uint16_t>(10000000)});

    static auto accel = sensors::BNO055(
        i2c::I2C_Config{
            .name = "BNO055", .dev_addr = 0x28, .scl_speed_hz = static_cast<uint16_t>(100000)},
        bus_handle);

    static auto bar1 = sensors::BME280(
        i2c::I2C_Config{
            .name = "BME280", .dev_addr = 0x76, .scl_speed_hz = static_cast<uint16_t>(100000)},
        bus_handle);

    static auto bar2 = sensors::BME280(
        i2c::I2C_Config{
            .name = "BME280-2", .dev_addr = 0x77, .scl_speed_hz = static_cast<uint16_t>(100000)},
        bus_handle);

    // Main Loop: Write latest data to the SD Card at 20hz
    char oriBuffer[ORIENTATION_BUFFER_MAX];
    while (true) {
        const auto eulerData = accel.getEuler();
        sdcard.write(sensors::FileName::ORIENT, "%f,%f,%f\n", eulerData.pitch, eulerData.roll,
                     eulerData.heading);
        sdcard.write(sensors::FileName::TEMP, "%f\n", bar1.getTemperature());
        sdcard.write(sensors::FileName::PRESSURE, "%f\n", bar1.getPressure());
        sdcard.write(sensors::FileName::HUMIDITY, "%d\n", bar1.getHumidity());

        // Display Orientation data on the OLED and send it to the radio
        snprintf(oriBuffer, sizeof(oriBuffer), ORIENTATION_FORMAT, eulerData.pitch, eulerData.roll,
                 eulerData.heading);
        D_LOGI(TAG, "%s", oriBuffer);
        udpClient.send(oriBuffer);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

extern "C" void app_main() {
    initArduino();
    display::init_display();
    D_LOGI(TAG, "Display Initialized");

    wifi::init_wifi();

#if defined(CONFIG_BENCH_BUILD_ENABLED)
    initBench();
#elif defined(CONFIG_RADIO_BUILD_ENABLED)
    initRadio();
#endif
}
