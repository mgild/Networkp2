#include "Server.h"
#include "stringutils.h"
#include <algorithm>
#include <cassert>

using namespace std;

Server::Server(const char* ip, int port) {
    s = make_unique<HTTPHandler>(ip, port);
    // connect proxy to temp cdn
    socktoserver->connect();
}

Server::~Server() {
}

void Server::start() {
    s->bind();
    s->listen(100);
    fd_set readSet;
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
			if (FD_ISSET((*it)->get_sd(), &readSet)) {
                if (handle_request(*it) < 0) {
                    it = clients.erase(it);
                }
			}
		}
    }
}


int Server::handle_request(const unique_ptr<Socket>& client) {
    string res = client->recv();
    if(!res.size()) {
        cout << "Connection closed" << endl;
        return -1;
    }
    cout << res << endl;
    assert(res.substr(res.size() - 4, res.size()) == "\r\n\r\n");
    // Setting up response
    vector<string> headers = explode("\r\n", res);
    vector<string> meta = explode(" ", headers[0]);
    cout << meta[1] << endl;
    //TODO : temporary. the favicon request times out. start in diff threads
    if (meta[1] == "/favicon.ico") {
        return 0;
    }
    meta[1] = meta[1];
    headers[0] = join(meta, " ");
    res = join(headers, "\r\n") + "\r\n\r\n";
    assert(res.substr(res.size() - 4, res.size()) == "\r\n\r\n");
    getCDN()->send(res);
    string resp = getCDN()->recv();
    if (!resp.size()) {
        cerr << "lost connection to server" << endl;
        return -1;
    }
    client->send(resp);
    return 0;
}

