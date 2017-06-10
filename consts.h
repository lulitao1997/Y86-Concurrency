//
// Created by nn on 2017/6/7.
//

#ifndef Y86_CONC_NET_CONST_H
#define Y86_CONC_NET_CONST_H

#include <string>
const int webserv_port = 8000;
const int fetch_port = 8001;
const int decode_port = 8002;
const int execute_port = 8003;
const int memory_port = 8004;
const int writeback_port = 8005;

#define prt_var(x) cout << #x << ": " << (x) << endl;


#endif //Y86_CONC_NET_CONST_H
