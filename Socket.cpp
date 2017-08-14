#include "Socket.h"
#include <string>
#include <cstring>
#include <cctype>
using namespace std;

const string ErrorMSG::kSyntaxError = "Error: option does not exist";
const string ErrorMSG::kArgError = "Error: missing or additional arguments";
const string ErrorMSG::kPortError = "Error: port number must be in the range 1024 to 65535";


Socket::Socket():sd(-1) {
    memset(&sin, 0, sizeof(sin));
}

Socket* Socket::accept() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int td;
    td=::accept(sd, (struct sockaddr*)&client_addr, &client_len);
    if (td < 0) {
        perror("Error accepting");
        exit(1);
    }
    return sock_factory(td, client_addr);
}



void Socket::close() {
    ::close(sd);
}

Socket::Socket(const char* ip, int port) {
    if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Error openning socket");
        abort();
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    if (!ip) {
        sin.sin_addr.s_addr = INADDR_ANY;
    } else {
        sin.sin_addr.s_addr = inet_addr(ip);
    }
    sin.sin_port = htons(port);
}


Socket::~Socket() {
    close();
}

void Socket::connect() {
    if (::connect(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("connect");
        printf("Cannot connect to address");
        abort();
    }
}


void Socket::bind() {
    // reserves the port
    if (::bind(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        printf("Cannot bind socket to address");
        abort();
    }
}

void Socket::listen(int num_connections) {
    // listens for incoming messages on given port
    if (::listen(sd, num_connections) < 0) {
        perror("Error listening");
        exit(1);
    }
}


void Socket::send(string msg) {
    if (::send(get_sd(),msg.c_str(), msg.size(), 0) < 0) {
        cerr << "Error sending" << endl;
        exit(1);
    }
}



string Socket::recv() {
    string res;
    int buf_len = 1024;
    char buf[buf_len];
    int len_recv = 0;
    if ((len_recv = ::recv(get_sd(), buf, buf_len-1, 0)) > 0) {
        buf[len_recv] = 0;
        //assert(strlen(buf) == len_recv);
        res += string(buf, len_recv);
    }
    return res;
}


