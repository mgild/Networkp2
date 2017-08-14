#include "Server.h"
#include <algorithm>
#include <cassert>

using namespace std;

Server::Server(const char* ip, int port) {
    s = new HTTPServerSocket(port);
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
                map<string,string> res = client->recv();
                for (auto t : res) {
                    cout << t.first << " :: " << t.second << endl;
                }
				if(!res.size()) {
					std::cout << "Connection closed" << std::endl;
					it = clients.erase(it);
                    delete client;
                    client = nullptr;
				} else {
                    HTTPClientSocket socktoserver("127.0.1.1", 80);
                    cout << "Huh" << endl;
                    socktoserver.connect();
                    socktoserver.send("GET /vod/1000Seg2-Frag3 HTTP/1.1\r\nHost: localhost\r\n\r\n");
                    cout << "VVVVVVVVVVVVVVV" << endl;
                    map<string,string> resp = socktoserver.recv();
                    cout << "VVVVVVVVVVVVVVV" << endl;
                    for (auto t : resp) {
                        cout << t.first << " :: " << t.second << endl;
                    }
                    //client->send("a");
                }
			}
		}
    }
}
