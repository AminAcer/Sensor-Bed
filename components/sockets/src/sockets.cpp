#include "sockets.h"

#include <pthread.h>

#include <cstddef>
#include <cstring>

#include "constants/general.h"
#include "esp_log.h"
#include "lwip/sockets.h"

static const char* TAG = "SOCKETS";

udp_socket* create_udp_socket(socket_type type, const char* ip, int port) {
    udp_socket* udp = (udp_socket*)malloc(sizeof(udp_socket));

    // Create a socket
    if ((udp->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        ESP_LOGE(TAG, "Socket creation failed!");
        free(udp);
        return NULL;
    }

    // Create a socket
    memset(&udp->server_addr, 0, sizeof(sockaddr_in));
    udp->server_addr.sin_family = AF_INET;
    udp->server_addr.sin_addr.s_addr = inet_addr(ip);
    udp->server_addr.sin_port = htons(port);
    udp->addr_len = sizeof(udp->server_addr);

    udp->callback = NULL;  // Init Callback to NULL

    if (type == socket_type::SERVER) {
        if (bind(udp->sockfd, (struct sockaddr*)&udp->server_addr, sizeof(sockaddr_in)) < 0) {
            ESP_LOGE(TAG, "Server bind failed!");
            close(udp->sockfd);
            free(udp);
            return NULL;
        }
    }

    return udp;
}

int send_udp(udp_socket* client, const char* packet) {
    return sendto(client->sockfd, packet, strlen(packet), 0, (struct sockaddr*)&client->server_addr,
                  client->addr_len);
}

static void* receive_udp_thread(void* arg) {
    udp_socket* server = (udp_socket*)arg;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        int recv_len = recvfrom(server->sockfd, buffer, BUFFER_SIZE - 1, 0,
                                (struct sockaddr*)&client_addr, &client_len);
        if (recv_len > 0) {
            buffer[recv_len] = '\0';
            if (server->callback) {
                server->callback(buffer);
            }
        } else {
            ESP_LOGE(TAG, "Received failed!");
        }
    }
    return NULL;
}

void start_receive_udp(udp_socket* server, void (*callback)(const char*)) {
    server->callback = callback;

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, receive_udp_thread, (void*)server) != 0) {
        ESP_LOGE(TAG, "Failed to create a thread!");
    }
    pthread_detach(thread_id);
}

void destroy_udp_socket(udp_socket* udp) {
    if (udp) {
        close(udp->sockfd);
        free(udp);
    }
}
