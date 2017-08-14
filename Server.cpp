#include "Server.h"
#include <algorithm>
#include <cassert>

using namespace std;

Server::Server(const char* ip, int port) {
    s = new HTTPHandler(ip, port);
}

Server::~Server() {
    delete s;
}

void Server::start() {
    s->bind();
    s->listen(100);
    fd_set readSet;
    while (true) {
		// Set up the readSet
		FD_ZERO(&readSet);
		FD_SET(s->get_sd(), &readSet);
		for (auto client : clients) {
			FD_SET(client->get_sd(), &readSet);
		}
		int maxfd = 0;
		if(clients.size() > 0) {
			maxfd = (*max_element(clients.begin(), clients.end(),
                [] (const Socket* lhs, const Socket* rhs) {
                    return lhs->get_sd() < rhs->get_sd();
                }))->get_sd();
		}
		maxfd = max(maxfd, s->get_sd());
		// maxfd + 1 is important
		int err = select(maxfd + 1, &readSet, NULL, NULL, NULL);
		assert(err != -1);
		if(FD_ISSET(s->get_sd(), &readSet)) {
            clients.push_back(s->accept());
		}
		for(auto it = clients.begin(); it != clients.end(); ++it) {
            Socket* client = *it;
			if(FD_ISSET(client->get_sd(), &readSet)) {
                string res = client->recv();
                cout << res << endl;
				if(!res.size()) {
					std::cout << "Connection closed" << std::endl;
					it = clients.erase(it);
                    delete client;
                    client = nullptr;
				} else {
                    HTTPHandler socktoserver("127.0.1.1", 80);
                    socktoserver.connect();
                    socktoserver.send(res);
//                    socktoserver.send("GET /vod/1000Seg2-Frag3 HTTP/1.1\r\nHost: localhost\r\n\r\n");
                    string resp = socktoserver.recv();
                    cout << resp << endl;
                    //client->send("a");
                }
			}
		}
    }
}
