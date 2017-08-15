#ifndef Server_h__
#define Server_h__

#include "HTTPHandler.h"
#include <sys/select.h>
#include <list>
#include <memory>


class Server {
    std::list<std::unique_ptr<Socket>> clients;
    std::unique_ptr<Socket> s;
    std::unique_ptr<Socket> socktoserver = make_unique<HTTPHandler>("127.0.1.1", 80);
    std::vector<int> bitrates;

public:
    Server(const char* ip, int port);


    ~Server();

    void start();

    int handle_request(const std::unique_ptr<Socket>& client);

    const std::unique_ptr<Socket>& getCDN() {
        return socktoserver;
    }

    std::string calculate_new_url(const std::string&);

    void load_video_info();

//    void handleMsg(Socket* client)
};

#endif
