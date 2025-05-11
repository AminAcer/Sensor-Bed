#include <Arduino.h>
#include <Wire.h>

#include "dis/dis.h"
#include "driver/i2c_types.h"
#include "i2c/i2c.h"
#include "logger/logger.h"
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

    static i2c_master_bus_handle_t bus_handle;
    i2c::setup_bus(&bus_handle, GPIO_NUM_48, GPIO_NUM_47, 0x00);
}
