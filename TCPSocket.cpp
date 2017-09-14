#include "TCPSocket.h"


using namespace std;


unique_ptr<TCPSocket> TCPSocket::accept() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int td;
    td=::accept(get_sd(), (struct sockaddr*)&client_addr, &client_len);
    if (td < 0) {
        perror("Error accepting");
        exit(1);
    }
    return make_unique<TCPSocket>(td, client_addr);
}


void TCPSocket::send(std::string msg) {
    if (::send(get_sd(),msg.c_str(), msg.size(), 0) < 0) {
        cerr << "Error sending" << endl;
        exit(1);
    }
}


void TCPSocket::listen(int num_connections) {
    // listens for incoming messages on given port
    if (::listen(get_sd(), num_connections) < 0) {
        perror("Error listening");
        exit(1);
    }
}




string TCPSocket::recv() {
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



