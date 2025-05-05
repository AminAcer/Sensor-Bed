#pragma once

#include <IPAddress.h>
#include <lwip/sockets.h>

#include <functional>

namespace sockets::udp {
    enum class SocketType { SERVER = 1, CLIENT = 2 };

    class Socket {
       public:
        /// @brief Constructor for a UDP socket
        ///
        /// @param type: Type of socket
        /// @param ip: The target IP for this socket
        /// @param port: The target port for this socket
        Socket(SocketType type, const char* ip, int port);
        ~Socket();

        // === Rule Of 5 ===
        Socket(const Socket&) = default;                 // Copy constructor
        Socket& operator=(const Socket&) = default;      // Copy assignment
        Socket(Socket&&) noexcept = default;             // Move constructor
        Socket& operator=(Socket&&) noexcept = default;  // Move assignment

        /// @brief Sends a message using a UDP client
        ///
        /// @param client: Socket pointer to a Client
        /// @param packet: Packet to send to the client
        ///
        /// @return bool: Were all bytes sent? (True: yes | False: no)
        bool send(const char* packet);

        // @brief Socket FD
        SocketType type{};

        // @brief Socket FD
        int sockfd{};

        // @brief Socket address
        struct sockaddr_in server_addr{};

        // @brief Socket address length
        socklen_t addr_len{};

        // @brief Callback to handle a packet
        std::function<void(const char*)> callback;
    };

    /// @brief Starts the receive thread for a socket and sets the callback
    ///
    /// @param socket: The UDP socket to start the receive thread on
    void start_receive(Socket* socket);

    /// @brief Basic handle function for receiving UDP messages that just prints the packet
    ///
    /// @param msg: The packet to act on
    void basic_handle(const char* msg);
}  // namespace sockets::udp
