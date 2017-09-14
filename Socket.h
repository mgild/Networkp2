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



/* template<typename T, typename... Args> */
// std::unique_ptr<T> make_unique(Args&&... args) {
    // return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
/* } */

class Socket {
    private:
        int sd;
        struct sockaddr_in sin;

        Socket();

    protected:
        void set_sd(int _sd) {
            sd = _sd;
        }

    public:

        int get_sd() const {
            return sd;
        }

        struct sockaddr_in get_sin() const {
            return sin;
        }


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
};

#endif
