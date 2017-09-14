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


void Socket::close() {
    ::close(sd);
}

Socket::Socket(const char* ip, int port) {
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




