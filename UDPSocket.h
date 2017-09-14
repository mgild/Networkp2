#pragma once
#include "Socket.h"

class UDPSocket: public Socket {
    public:
        UDPSocket(const char* ip, int port): Socket(ip, port) {
            set_sd(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
            if (get_sd() < 0) {
                perror("Error openning socket");
                abort();
            }
        }


        std::string recvfrom(const std::unique_ptr<sockaddr_in>& src);

        void send(std::string msg, const std::unique_ptr<sockaddr_in>& dest);
};
