#pragma once

#include <lwip/sockets.h>

enum socket_type { SERVER = 1, CLIENT = 2 };

typedef struct udp_socket {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    void (*callback)(const char*);
} udp_socket;

/// @brief Creates a UDP socket
///
/// @param type: Type of socket
/// @param ip: The target IP for this socket
/// @param port: The target port for this socket
///
/// @return udp_socket*: Constructed UDP socket pointer
udp_socket* create_udp_socket(socket_type type, const char* ip, int port);

/// @brief Sends a message using a UDP client
///
/// @param client: Socket pointer to a Client
/// @param packet: Packet to send to the client
///
/// @return bool: Were all bytes sent? (True: yes | False: no)
bool send_udp(udp_socket* client, const char* packet);

/// @brief Starts the receive thread for a socket and sets the callback
///
/// @param socket: Socket pointer to start receiving on
/// @param *callback: Function pointer to call when a packet is received
void start_receive_udp(udp_socket* socket, void (*callback)(const char*));
