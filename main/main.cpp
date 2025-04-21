#include <Arduino.h>
#include <Wire.h>

#include "SSD1306Wire.h"
#include "constants/heltec_pins.h"
#include "display/display.h"
#include "sockets/sockets.h"
#include "wifi/wifi.h"

static const char* TAG = "MAIN";

// Display object for writing to the display
static SSD1306Wire display = SSD1306Wire(0x3C, HELTEC_SDA_OLED, HELTEC_SCL_OLED);

void handle_udp(const char* msg) {
    ESP_LOGI(TAG, "Received message: %s", msg);
    display_text(&display, msg, font_size::MEDIUM, 10, 0);
}

extern "C" void app_main() {
    initArduino();
    init_display(&display);
    display_text(&display, "init complete");
    init_wifi(&display);
    udp_socket* svr = create_udp_socket(socket_type::SERVER, "0.0.0.0", 46729);
    udp_socket* client = create_udp_socket(socket_type::CLIENT, "192.168.0.183", 36729);
    start_receive_udp(svr, handle_udp);
    send_udp(client, "Client test msg");
}
