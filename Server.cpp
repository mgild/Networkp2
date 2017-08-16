#include "Server.h"
#include "stringutils.h"
#include "logger.h"
#include <algorithm>
#include <cassert>
#include <chrono>
using namespace std;
using namespace std::chrono;

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
    load_video_info();
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
            clients.push_back(static_cast_ptr<HTTPHandler>(s->accept()));
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


/**
 * @Brief takes a client socket ready to be read from.  Should ideally wait
 * until the client socket is ready to be read from before calling.
 * Return 0 on success, -1 on error
 *
 * @Param client client socket to read and respond to
 *
 */
int Server::handle_request(const unique_ptr<HTTPHandler>& client) {
    auto started = high_resolution_clock::now();
    string res = client->recv();
    if(!res.size()) {
        cout << "Connection closed" << endl;
        return -1;
    }
    assert(res.substr(res.size() - 4, res.size()) == "\r\n\r\n");
    // Setting up response
    vector<string> headers = explode("\r\n", res);
    vector<string> meta = explode(" ", headers[0]);
    cout << meta[1] << endl;
    //TODO : temporary. the favicon request times out. start in diff threads
    if (meta[1] == "/favicon.ico") {
        return 0;
    }
    meta[1] = calculate_new_url(meta[1], client);
    headers[0] = join(meta, " ");
    res = join(headers, "\r\n") + "\r\n\r\n";
    assert(res.substr(res.size() - 4, res.size()) == "\r\n\r\n");
    const unique_ptr<HTTPHandler>& cdn = getCDN();
    cdn->send(res);
    string resp = cdn->recv();
    if (!resp.size()) {
        cerr << "lost connection to server" << endl;
        return -1;
    }
    auto done = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(done - started);
    double megabits = (1.0 * res.size() * 8)/1024/1024;
    double t_new = megabits/time_span.count();
    cout << t_new << " Mbps" << endl;
    cout << meta[1] << endl;
    logger::println(
                to_string(time_span.count()) + " " +
                to_string(t_new) + " " +
                to_string(client->get_throughput()) + " " +
                to_string(calculate_bitrate(client)) + " " +
                meta[1]);
    client->update_throughput(t_new);
    client->send(resp);
    return 0;
}


std::string Server::calculate_new_url(const std::string& path, const std::unique_ptr<HTTPHandler>& client) {
    if (path == "/vod/big_buck_bunny.f4m") {
        return "/vod/big_buck_bunny_nolist.f4m";
    }
    if (path.find("/vod/") == 0) {
        string basename = explode("/", path).back();
        string newpath = "/vod/";
        vector<string> sep1 = explode("-", basename);
        string frag = explode("Frag", sep1.back()).back();
        vector<string> sep2 = explode("Seg", sep1[0]);
        string bitrate = sep2[0];
        string seg = sep2[1];
        newpath += to_string(calculate_bitrate(client)) + "Seg" + seg + "-Frag" + frag;
        return newpath;
    }
    return path;
}


/**
 * @Brief loads video bitrates available
 */
void Server::load_video_info() {
    string req = "GET /vod/big_buck_bunny.f4m HTTP/1.1\r\n"
    "Host: localhost:11000\r\n"
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:54.0) Gecko/20100101 Firefox/54.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Referer: http://localhost:11000/StrobeMediaPlayback.swf\r\n"
    "Connection: keep-alive\r\n"
    "\r\n";
    const unique_ptr<HTTPHandler>& cdn = getCDN();
    cdn->send(req);
    string resp = cdn->recv();
    if (!resp.size()) {
        cerr << "lost connection to server" << endl;
        exit(1);
    }
    vector<string> lines = explode("\n", resp);
    for (string& line : lines) {
        if (line.find("bitrate") != string::npos) {
            int bitrate = stoi(explode("\"",line)[1]);
            cout << bitrate << endl;
            bitrates.push_back(bitrate);
        }
    }
}

//bitrates should be sorted
int Server::calculate_bitrate(const std::unique_ptr<HTTPHandler>& client) {
    double t_cur = client->get_throughput();
    int chosen = bitrates[0];
    for (int bitrate: bitrates) {
        if (t_cur >= 1.5*bitrate/1000) {
            chosen = bitrate;
        } else {
            break;
        }
    }
    return chosen;
}
