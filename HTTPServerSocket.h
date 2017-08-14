#pragma once

#include "Socket.h"


class HTTPServerSocket : public Socket {

    static constexpr char* kheader_delim = (char*)"\r\n\r\n";
    static const std::string kdelim;
    static const std::string kmethod_key
    static const std::string kproto_key;
    static const std::string kpath_key;
    static const std::string kbody_key
    static const std::string kbody_len_key;

public:


    HTTPServerSocket(int port):Socket(nullptr, port){}
    HTTPServerSocket(int sd, struct sockaddr_in sin):Socket(sd,sin){}

    Socket* socket_factory(const char* ip, int port) {
        return new HTTPServerSocket(port);
    }

    Socket* socket_factory(int sd, struct sockaddr_in sin) {
        return new HTTPServerSocket(sd,sin);
    }


    std::map<std::string, std::string> recv();

    void send(std::string);

    void send(std::map<std::string,std::string> d);

private:

    std::map<std::string,std::string> gen_header(const std::string& hdr);
};
