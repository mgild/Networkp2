#include "HTTPServerSocket.h"
#include "stringutils.h"
#include <sstream>
using namespace std;


#include <algorithm>
#include <cctype>
#include <locale>

const string HTTPServerSocket::kdelim = "\r\n";
const string HTTPServerSocket::kmethod_key = "Method";
const string HTTPServerSocket::kproto_key = "Protocol";
const string HTTPServerSocket::kpath_key = "Path";
const string HTTPServerSocket::kbody_key = "Content";
const string HTTPServerSocket::kbody_len_key = "Content-Length";



map<string,string> HTTPServerSocket::gen_header(const string& hdr) {
    map<string,string> res;
    const string key_val_delim = ":";
    vector<string> lines = explode(kdelim, hdr);
    for (int i = 0; i < (int)lines.size(); ++i) {
        string& line = lines[i];
        trim(line);
        if (!line.size()) continue;
        size_t pos = line.find(key_val_delim);
        if (pos != string::npos) {
            string key = line.substr(0,pos);
            string val = string(line.begin() + pos + key_val_delim.size(), line.end());
            trim(key);
            trim(val);
            res[key] = val;
        } else {
            vector<string> meta;
            istringstream iss(line);
            for(string s; iss >> s;) {
                trim(s);
                if (s.size()) {
                    meta.push_back(s);
                }
            }
            res[kmethod_key] = meta[0];
            res[kpath_key] = meta[1];
            res[kproto_key] = meta[2];
        }

    }
    return res;
}


map<string,string> HTTPServerSocket::recv() {
    map<string,string> ret;
    string res;
    int buf_len = 1024;
    char buf[buf_len];
    int len_recv = 0;
    char* div = 0;
    cout << "---" << endl;
    while (!div && (len_recv = ::recv(get_sd(), buf, buf_len-1, 0)) > 0) {
        buf[len_recv] = 0;
        div = strstr(buf, kheader_delim);
        if (div) *div = 0;
        res += buf;
        cout << "!!!" << buf << endl;
    }
    div += strlen(kheader_delim);
    ret = gen_header(res);
    if (ret.find(kbody_len_key) != ret.end()) {
        int body_len = stoi(ret[kbody_len_key]) - strlen(div);
        string body = div;
        while (body_len && (len_recv = ::recv(get_sd(), buf, buf_len-1, 0)) > 0) {
            body_len -= len_recv;
            buf[len_recv] = 0;
            body += buf;
            cout << "!!!" << buf << endl;
        }
        ret[kbody_key] = body;
        res += body;
    }
    return ret;
}

void HTTPServerSocket::send(string msg) {
    if (::send(get_sd(),msg.c_str(), msg.size(), 0) < 0) {
        cerr << "Error sending" << endl;
        exit(1);
    }
}

void HTTPServerSocket::send(map<string,string> d) {
    string msg;
    msg += d[kmethod_key] + " " + d[kpath_key] + d[kproto_key] + kdelim;
    d.erase(kmethod_key);
    d.erase(kpath_key);
    d.erase(kproto_key);
    string body = d[kbody_key];
    d.erase(kbody_key);
    for (auto& p : d) {
        msg += p.first + ": " + p.second + kdelim;
    }
    msg += kdelim + kdelim;
    msg += body;
    send(msg);
}


