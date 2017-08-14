#pragma once

#include "Socket.h"


class HTTPClientSocket : public Socket {
private:


public:
    HTTPClientSocket(const char* ip, int port):Socket(ip,port){}

    HTTPClientSocket(int sd, struct sockaddr_in sin):Socket(sd,sin){}


    Socket* socket_factory(const char* ip, int port) {
        return new HTTPClientSocket(ip, port);
    }
    Socket* socket_factory(int sd, struct sockaddr_in sin){
        return new HTTPClientSocket(sd,sin);
    }



    std::map<std::string,std::string> recv();
    void send(std::string msg);
    void send(std::map<std::string,std::string> d);
};

