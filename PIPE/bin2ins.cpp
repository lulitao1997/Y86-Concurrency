#include "bin2ins.h"
string strip(const string &s) {
    string ans;
    for (auto i:s) if (i!=' ') ans+=i;
    return ans;
}
string strip_head(const string &s) {
    int a = -1;
    for (int i=0; i<s.length(); i++)
        if (s[i]==' ' || s[i]=='\t')
            a = i;
        else
            break;
    return s.substr(a+1);
}

pair<vector<ins>,map<int,int> > read_from_yo(const string &input) {
    vector<ins> V;
    map<int,int> addr2ln;
    fstream fin(input);
    string buf;
    cerr << input << endl;
    while (getline(fin,buf)) {
        //cerr <<"buf:" << buf << endl;
        if (buf.find(':')==string::npos) continue;
        stringstream s_in(buf);
        string tmp;
        getline(s_in,tmp,':');
        tmp = strip(tmp);
        if (tmp.empty()) continue;
        cerr << "tmp::" << tmp << "::" <<endl;
        int addr = 0;
        try {
            addr = stoi(string(tmp.begin()+2,tmp.end()),nullptr,16);
        }
        catch(...) {
            continue;
        }
        getline(s_in,tmp,'|');
        tmp = strip(tmp);
        //if (tmp.empty()) continue;
        string code = tmp;
        getline(s_in,tmp,'#');
        //tmp = tmp.substr(tmp.find_last_of(' '));
        string content = tmp;//strip_head(tmp);
        addr2ln[addr] = V.size();
        V.push_back(ins(addr, code, content));
    }
    cerr << "read_from_yo called!" << endl;
    for (auto i:V) {
        cerr << ">>" <<i.addr << ',' << i.code << ',' << i.content << endl;
    }

    return make_pair(V,addr2ln);
}
