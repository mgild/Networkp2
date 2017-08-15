#ifndef Server_h__
#define Server_h__

#include "HTTPHandler.h"
#include <sys/select.h>
#include <list>
#include <memory>


class Server {
    std::list<std::unique_ptr<Socket>> clients;
    std::unique_ptr<Socket> s;

public:
    Server(const char* ip, int port);


    ~Server();

    void start();

//    void handleMsg(Socket* client)
};

#endif
