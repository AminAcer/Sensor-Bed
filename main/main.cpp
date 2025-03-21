#include <Arduino.h>
#include <Wire.h>

#include "SSD1306Wire.h"
#include "constants/heltec_pins.h"
#include "display.h"
#include "sockets.h"
#include "wifi.h"

static const char* TAG = "MAIN";

// Display object for writing to the display
static SSD1306Wire display = SSD1306Wire(0x3C, HELTEC_SDA_OLED, HELTEC_SCL_OLED);

void handle_udp(const char* msg) {
    ESP_LOGI(TAG, "Received message: %s", msg);
    display_text(&display, msg, 10, 0);
}

extern "C" void app_main() {
    initArduino();
    init_display_pins();
    display.init();
    display.setFont(ArialMT_Plain_16);
    display_text(&display, "init complete");
    init_wifi(&display);
    udp_socket* svr = create_udp_socket(SERVER, "0.0.0.0", 46729);
    udp_socket* client = create_udp_socket(CLIENT, "192.168.0.183", 36729);
    start_receive_udp(svr, handle_udp);
    send_udp(client, "Client test msg");
}
