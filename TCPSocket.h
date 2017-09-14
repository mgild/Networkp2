#pragma once
#include "Socket.h"

class TCPSocket : public Socket {
 public:

    TCPSocket(const char* ip, int port): Socket(ip, port) {
        set_sd(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        if (get_sd() < 0) {
            perror("Error openning socket");
            abort();
        }
    }


    void listen(int num_connections);

    virtual void send(std::string msg);

    std::unique_ptr<TCPSocket> accept();

    virtual std::string recv();
};
