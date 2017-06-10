//
// Created by nn on 2017/6/8.
//

#ifndef Y86_CONC_MAP_IO_H
#define Y86_CONC_MAP_IO_H

#include "consts.h"
#include <map>
#include <string>
#include <sstream>
using namespace std;
void map_read(map<string, string> &m, string str) {
    for (auto &c : str) {
        if (c == ':' || c == '{' || c == '}' || c == '"') c = ' ';
        if (c == ',') c = '\n';
    }
    istringstream Sin(str);
    string key, value;
    while (Sin >> key >> value) {
        m[key] = value;
    }
}

string map2json(const map<string, string> &m) {
    string ans = "{";
    for (auto i = m.begin(); i!=m.end(); ++i) {
        if (i != m.begin()) ans+= ',';
        ans += '"' + i->first + "\":\"" + i->second + '"';
    }
    return ans + '}';
}
#endif //Y86_CONC_MAP_IO_H
