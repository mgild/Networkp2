#include "Server.h"
#include "stringutils.h"
#include <algorithm>
#include <cassert>

using namespace std;

Server::Server(const char* ip, int port) {
    s = make_unique<HTTPHandler>(ip, port);
}

Server::~Server() {
}

void Server::start() {
    s->bind();
    s->listen(100);
    fd_set readSet;
    HTTPHandler socktoserver("127.0.1.1", 80);
    socktoserver.connect();
    while (true) {
		// Set up the readSet
		FD_ZERO(&readSet);
		FD_SET(s->get_sd(), &readSet);
		for (auto& client : clients) {
			FD_SET(client->get_sd(), &readSet);
		}
		int maxfd = 0;
        for (auto& client: clients) {
            maxfd = max(maxfd, client->get_sd());
        }
		maxfd = max(maxfd, s->get_sd());
		// maxfd + 1 is important
		int err = select(maxfd + 1, &readSet, NULL, NULL, NULL);
		assert(err != -1);
		if(FD_ISSET(s->get_sd(), &readSet)) {
            clients.push_back(s->accept());
		}
		for(auto it = clients.begin(); it != clients.end(); ++it) {
			if(FD_ISSET((*it)->get_sd(), &readSet)) {
                string res = (*it)->recv();
				if(!res.size()) {
					std::cout << "Connection closed" << std::endl;
					it = clients.erase(it);
				} else {
                    cout << res << endl;
                    assert(res.substr(res.size() - 4, res.size()) == "\r\n\r\n");
                    // Setting up response
                    vector<string> meta = explode("\r\n", res);
                    string path = explode(" ", meta[0])[1];
                    cout << path << endl;
                    //TODO : temporary. the favicon request times out. start in diff threads
                    if (path == "/favicon.ico") {
                        continue;
                    }
                    socktoserver.send(res);
                    string resp = socktoserver.recv();
                    (*it)->send(resp);
                }
			}
		}
    }
}
