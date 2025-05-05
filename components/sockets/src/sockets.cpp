#include "sockets/sockets.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/idf_additions.h>
#include <freertos/projdefs.h>
#include <freertos/task.h>

#include <cstddef>
#include <cstring>
#include <memory>

#include "constants/general.h"
#include "display/display.h"

namespace sockets::udp {
    static const char* TAG = "SOCKETS";

    Socket::Socket(SocketType type, const char* ip, int port) {
        // Create a socket
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            ESP_LOGE(TAG, "Socket creation failed!");
        }

        // Create a socket
        memset(&server_addr, 0, sizeof(sockaddr_in));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip);
        server_addr.sin_port = htons(port);
        addr_len = sizeof(server_addr);

        if (type == SocketType::SERVER) {
            if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(sockaddr_in)) < 0) {
                ESP_LOGE(TAG, "Server bind failed!");
                close(sockfd);
            }
        }
    }

    Socket::~Socket() {
        close(sockfd);
    }

    bool Socket::send(const char* packet) {
        return sendto(sockfd, packet, strlen(packet), 0, (struct sockaddr*)&server_addr,
                      addr_len) == strlen(packet);
    }

    void basic_handle(const char* msg) {
        ESP_LOGI(TAG, "Received message: %s", msg);
        display::display_text(msg, display::FontSize::MEDIUM, 10, 0);
    }

    void receive_thread(void* arg) {
        Socket* socket = (Socket*)arg;
        char buffer[BUFFER_SIZE];
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        while (true) {
            int recv_len = recvfrom(socket->sockfd, buffer, BUFFER_SIZE - 1, 0,
                                    (struct sockaddr*)&client_addr, &client_len);
            if (recv_len > 0) {
                buffer[recv_len] = '\0';
                if (socket->callback) {
                    socket->callback(buffer);
                }
            } else {
                ESP_LOGE(TAG, "Received failed!");
            }

            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Cleanup if loop exists
        vTaskDelete(NULL);
    }

    void start_receive(Socket* socket) {
        if (socket == nullptr) {
            return;
        }

        xTaskCreatePinnedToCore(receive_thread, "Server Task",
                                4096,           // Allocate 16KB to the stack of this task
                                (void*)socket,  // Pass the socket argument in
                                5, NULL, 1);
    }

}  // namespace sockets::udp
