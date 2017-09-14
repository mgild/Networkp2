#include "UDPSocket.h"
using namespace std;

int main() {
    UDPSocket s(NULL, 6000);
    s.bind();
    while (1) {
        unique_ptr<sockaddr_in> sin(new sockaddr_in);
        string recv = s.recvfrom(sin);
        cout << recv << endl;
        s.send("ack", sin);
    }
    return 0;
}
