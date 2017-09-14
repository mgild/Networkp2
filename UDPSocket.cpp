#include "UDPSocket.h"

using namespace std;


string UDPSocket::recvfrom(const unique_ptr<sockaddr_in>& src) {
    string res;
    int buf_len = 1024;
    char buf[buf_len];
    sockaddr_in addr;
    socklen_t addr_size = sizeof(sockaddr_in);
    int byte_count = ::recvfrom(get_sd(), buf, buf_len - 1, 0,(struct sockaddr*)&addr, &addr_size);
    if (byte_count > 0) {
        res = string(buf, byte_count);
    }
    memcpy(src.get(), &addr, addr_size);
    return res;
}

void UDPSocket::send(string msg, const unique_ptr<sockaddr_in>& dest) {
    socklen_t addrsize = sizeof(sockaddr_in);
    ssize_t status = ::sendto(
            get_sd(),
            msg.c_str(),
            msg.size(),
            0,
            (sockaddr*)dest.get(),
            addrsize);
    if (status < 0) {
        cerr << "Error sending" << endl;
        exit(1);
    }
}

