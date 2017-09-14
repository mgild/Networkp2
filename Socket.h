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
#include <memory>

struct ErrorMSG {
    static const std::string kSyntaxError ;
    static const std::string kArgError ;
    static const std::string kPortError ;
};



template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Socket {
    private:
        int sd;
        struct sockaddr_in sin;

        Socket();


    public:

        virtual std::unique_ptr<Socket> sock_factory(const char* ip, int port) {
            return make_unique<Socket>(ip, port);
        }

        virtual std::unique_ptr<Socket> sock_factory(int sd, struct sockaddr_in sin) {
            return make_unique<Socket>(sd,sin);
        }

        int get_sd() const {
            return sd;
        }

        std::unique_ptr<Socket> accept();

        virtual std::string recv();

        virtual std::string recvfrom(const std::unique_ptr<sockaddr_in>& src);

        virtual void send(std::string msg);

        virtual void send(std::string msg, const std::unique_ptr<sockaddr_in>& dest);

        void close();

        /**
         * @Brief Create a passive socket listener; TCP
         *
         * @Param ip IP to attach to, INADDR_ANY if NULL
         * @Param port port to listen on
         * @Param num_connections max # of tcp connections allowed to wait
         *
         */
        Socket(const char* ip, int port, const std::string& mode="TCP");
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
