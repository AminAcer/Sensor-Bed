#include <Arduino.h>
#include <Wire.h>

#include "display.h"
#include "sockets.h"
#include "wifi.h"

static const char* TAG = "MAIN";

void handle_udp(const char* msg) {
    ESP_LOGI(TAG, "Received message: %s", msg);
}

extern "C" void app_main() {
    initArduino();
    SSD1306Wire display = initDisplay();
    display.init();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "testing");
    display.display();
    init_wifi();
    udp_socket* svr = create_udp_socket(SERVER, "0.0.0.0", 46729);
    udp_socket* client = create_udp_socket(CLIENT, "192.168.0.183", 36729);

    start_receive_udp(svr);
}
