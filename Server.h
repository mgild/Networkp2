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

public:
    Server(const char* ip, int port);


    ~Server();

    void start();

    int handle_request(const std::unique_ptr<Socket>& client);

    const std::unique_ptr<Socket>& getCDN() {
        return socktoserver;
    } 

//    void handleMsg(Socket* client)
};

#endif
