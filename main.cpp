#include "Server.h"
#include "logger.h"
#include <cstdlib>

using namespace std;
//./miProxy <log> <alpha> <listen-port> <dns-ip> <dns-port> [<www-ip>]
int main(int argc, char** argv) {
    assert(argc >= 6);
    logger::set_file(argv[1]);
    HTTPHandler::set_alphas(atof(argv[2]));
    Server s(nullptr, stoi(argv[3]));
    string dns_ip = argv[4];
    int dns_port = stoi(argv[5]);
    if (argc > 6) {
        string cdnip = argv[6];
        s.set_cdn_ip(cdnip);
    }
    s.start();
}
