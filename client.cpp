#include "UDPSocket.h"

using namespace std;

int main(int argc, char *argv[]) {
    UDPSocket s("0.0.0.0", 0);
    struct sockaddr_in server= UDPSocket("0.0.0.0", 6000).get_sin();
    unique_ptr<struct sockaddr_in> sin_ptr(new struct sockaddr_in(server));
    s.send("A", sin_ptr);
    unique_ptr<struct sockaddr_in> ptr(new struct sockaddr_in(server));
    cout << s.recvfrom(ptr) << endl;
    return 0;
}
