#include "stringutils.h"
#include <algorithm>
#include <string>
#include <cctype>

using namespace std;

// trim from start (in place)
void ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return !isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
string ltrim_copy(string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
string rtrim_copy(string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
string trim_copy(string s) {
    trim(s);
    return s;
}

vector<string> explode( const string &delimiter, const string &str)
{
    vector<string> arr;
 
    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change
 
    int i=0;
    int k=0;
    while(i < strleng) {
        int j=0;
        while (i+j < strleng && j < delleng && str[i+j] == delimiter[j])
            j++;
        if (j == delleng)//found delimiter
        {
            arr.push_back(str.substr(k, i-k));
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(str.substr(k, i-k));
    auto end = remove_if(arr.begin(), arr.end(),
                         [](const string& s)
                         { return s.size() == 0; });
    return {arr.begin(), end};
}


string merge(const vector<string>& v, const string& delim) {
    string res;
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i) {
            res += delim;
        }
        res += v[i];
    }
    return res;
}
