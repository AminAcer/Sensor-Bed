#include <Arduino.h>
#include <Wire.h>

#include <memory>

#include "constants/heltec_pins.h"
#include "display/display.h"
#include "sockets/sockets.h"
#include "wifi/wifi.h"

extern "C" void app_main() {
    initArduino();
    display::init_display();
    display::display_text("Display Initialized");

    init_wifi();

    auto svr = sockets::udp::Socket(sockets::udp::SocketType::SERVER, "0.0.0.0", 46729);
    auto client = sockets::udp::Socket(sockets::udp::SocketType::CLIENT, "192.168.0.183", 36729);

    svr.callback = sockets::udp::basic_handle;
    sockets::udp::start_receive(&svr);
    client.send("Client test msg");
}
