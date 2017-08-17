#pragma once
#include "Socket.h"
#include <map>
#include <string>
#include <iostream>


class HTTPHandler : public Socket{
private:
    static const std::string kdelim;
    static const std::string kmethod_key;
    static const std::string kproto_key;
    static const std::string kpath_key;
    static const std::string kbody_key;
    static const std::string kbody_len_key;

    // current throughput rate into this socket
    double T_cur;
    static double alpha;
public:

    HTTPHandler(const char* ip, int port):Socket(ip,port),T_cur(0) {
    }

    HTTPHandler(int sd, struct sockaddr_in sin):Socket(sd,sin),T_cur(0){}

    virtual std::unique_ptr<Socket> sock_factory(const char* ip, int port) override {
        return make_unique<HTTPHandler>(ip, port);
    }
    virtual std::unique_ptr<Socket> sock_factory(int sd, struct sockaddr_in sin) override {
        return make_unique<HTTPHandler>(sd,sin);
    }
    
    std::map<std::string, std::string> gen_header(const std::string& hdr);

    std::string recv();

    double get_throughput() {
        return T_cur;
    }

    void update_throughput(double t_new) {
        T_cur = alpha * t_new + (1 - alpha) * T_cur;
        std::cout << "Estimated Throughput: " << T_cur << std::endl;
    }

    static void set_alphas(double a) {
        assert(a >= 0 && a <= 1);
        alpha = a;
    }

};
