#ifndef Server_h__
#define Server_h__

#include "HTTPHandler.h"
#include <sys/select.h>
#include <list>
#include <memory>


template<typename D, typename B>
std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>&& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}

class Server {
    std::list<std::unique_ptr<HTTPHandler>> clients;
    std::unique_ptr<HTTPHandler> s;
    std::unique_ptr<HTTPHandler> socktoserver = make_unique<HTTPHandler>("10.0.0.1", 80);
    std::vector<int> bitrates;

public:
    Server(const char* ip, int port);


    ~Server();

    void start();

    int handle_request(const std::unique_ptr<HTTPHandler>& client);

    const std::unique_ptr<HTTPHandler>& getCDN() {
        return socktoserver;
    }

    std::string calculate_new_url(const std::string&, const std::unique_ptr<HTTPHandler>& client);

    void load_video_info();

    int calculate_bitrate(const std::unique_ptr<HTTPHandler>& client);
//    void handleMsg(Socket* client)

    void set_cdn_ip(const std::string& ip) {
        socktoserver = make_unique<HTTPHandler>(ip.c_str(), 80);
    }
};

#endif
