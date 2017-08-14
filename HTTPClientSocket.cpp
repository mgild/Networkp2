#include "HTTPClientSocket.h"

using namespace std;

map<string,string> HTTPClientSocket::recv() {
    map<string,string> ret;
    string res;
    int buf_len = 1024;
    char buf[buf_len];
    int len_recv = 0;
    char* div = 0;
    cout << "---" << endl;
    while (!div && (len_recv = ::recv(get_sd(), buf, buf_len-1, 0)) > 0) {
        buf[len_recv] = 0;
        //div = strstr(buf, kheader_delim);
        if (div) *div = 0;
        res += buf;
        cout << "!!!" << buf << endl;
    }
    return ret;
}

void HTTPClientSocket::send(string msg) {
    if (::send(get_sd(),msg.c_str(), msg.size(), 0) < 0) {
        cerr << "Error sending" << endl;
        exit(1);
    }
}

void HTTPClientSocket::send(map<string,string> d) {
    assert(0);
}
