#include "sockets.h"

#include "constants/general.h"
#include "esp_log.h"
#include "lwip/sockets.h"

const static char* TAG = "SOCKETS";

int create_udp_server() {
    struct sockaddr_in serverInfo, clientInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(46729);

    char readBuffer[1024] = {0};

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        ESP_LOGE(TAG, "Failed to create a socket!");
        return FAILURE;
    }

    if (bind(sock, (const struct sockaddr*)&serverInfo, sizeof(serverInfo)) != 0) {
        ESP_LOGE(TAG, "Failed to connect to %s!", inet_ntoa(serverInfo.sin_addr.s_addr));
        close(sock);
        return FAILURE;
    }

    ESP_LOGI(TAG, "Connected to UDP server.");
    while (1) {
        socklen_t len = sizeof(clientInfo);
        int n = recvfrom(sock, readBuffer, 1024, 0, (struct sockaddr*)&clientInfo, &len);
        readBuffer[n] = '\0';
        ESP_LOGI(TAG, "Received: %s\n", readBuffer);
    }

    return SUCCESS;
}
