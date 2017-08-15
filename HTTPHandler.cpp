#include "HTTPHandler.h"
#include "stringutils.h"
#include <sstream>
#include <list>
using namespace std;

const string HTTPHandler::kdelim = "\r\n";
const string HTTPHandler::kmethod_key = "Method";
const string HTTPHandler::kproto_key = "Protocol";
const string HTTPHandler::kpath_key = "Path";
const string HTTPHandler::kbody_key = "Content";
const string HTTPHandler::kbody_len_key = "Content-Length";

string HTTPHandler::recv() {
    string res;
    string buf;
    size_t delim_loc = 0;
    const string kheader_delim = kdelim+kdelim;
    do {
        buf = Socket::recv();
        delim_loc = buf.find(kheader_delim);
        if (delim_loc != string::npos) {
            res += buf.substr(0, delim_loc + kheader_delim.size());
            break;
        } else {
            res += buf;
        }
    } while(buf.size());
    map<string,string> header = gen_header(res);
    if (header.find(kbody_len_key) != header.end()) {
        assert(delim_loc != string::npos);
        int body_len = stoi(header[kbody_len_key]) - (buf.size() - delim_loc - kheader_delim.size());
        string body = buf.substr(delim_loc + kheader_delim.size());
        do {
            buf = Socket::recv();
            body_len -= buf.size();
            body += buf;
        } while (buf.size() && body_len);
        res += body;
        //if connection wasnt closed/no errors
        if (buf.size()) {
            assert((int)body.size() == stoi(header[kbody_len_key]));
        }
    }
    return res;

}



map<string,string> HTTPHandler::gen_header(const string& hdr) {
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
