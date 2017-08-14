#ifndef Server_h__
#define Server_h__

#include "HTTPServerSocket.h"
#include "HTTPClientSocket.h"
#include <sys/select.h>
#include <list>


class Server {
    std::list<Socket*> clients;
    Socket* s;

public:
    Server(const char* ip, int port);


    ~Server();

    void start();
};

#endif
