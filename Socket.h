#ifndef Socket_h__
#define Socket_h__

#include <ctime>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <string>
#include <cassert>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>

struct ErrorMSG {
    static const std::string kSyntaxError ;
    static const std::string kArgError ;
    static const std::string kPortError ;
};





class Socket {
    private:
        int sd;
        struct sockaddr_in sin;

        Socket();

    public:

        int get_sd() const {
            return sd;
        }

        Socket* accept();

        virtual std::string recv();
        virtual void send(std::string msg);
        void close();

        /**
         * @Brief Create a passive socket listener; TCP
         *
         * @Param ip IP to attach to, INADDR_ANY if NULL
         * @Param port port to listen on
         * @Param num_connections max # of tcp connections allowed to wait
         *
         */
        Socket(const char* ip, int port);
        Socket(int sd, struct sockaddr_in sin):sd(sd), sin(sin){}

        /**
         * @Brief Disable copy constructor and assignment
         *
         * @Param Socket
         */
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        virtual ~Socket();


        void connect();


        void bind();

        void listen(int num_connections);

};

#endif
