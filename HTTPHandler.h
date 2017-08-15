#pragma once
#include "Socket.h"
#include <map>
#include <string>



class HTTPHandler : public Socket{
private:
    static const std::string kdelim;
    static const std::string kmethod_key;
    static const std::string kproto_key;
    static const std::string kpath_key;
    static const std::string kbody_key;
    static const std::string kbody_len_key;
public:

    HTTPHandler(const char* ip, int port):Socket(ip,port) {
    }

    HTTPHandler(int sd, struct sockaddr_in sin):Socket(sd,sin){}

    virtual std::unique_ptr<Socket> sock_factory(const char* ip, int port) {
        return make_unique<HTTPHandler>(ip, port);
    }
    virtual std::unique_ptr<Socket> sock_factory(int sd, struct sockaddr_in sin) {
        return make_unique<HTTPHandler>(sd,sin);
    }
    
    std::map<std::string, std::string> gen_header(const std::string& hdr);

    std::string recv();

};
