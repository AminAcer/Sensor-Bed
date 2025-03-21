#pragma once

#include "lwip/sockets.h"

enum socket_type { SERVER = 1, CLIENT = 2 };

typedef struct udp_socket {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    void (*callback)(const char*);
} udp_socket;

/// @brief Creates a UDP socket
udp_socket* create_udp_socket(socket_type type, const char* ip, int port);

/// @brief Sends a message using a UDP client
int send_udp(udp_socket* client, const char* packet);

/// @brief Starts the receive thread for a socket and sets the callback
void start_receive_udp(udp_socket* socket, void (*callback)(const char*));
