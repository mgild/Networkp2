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


    std::map<std::string, std::string> gen_header(const std::string& hdr);

    std::string recv();

};
