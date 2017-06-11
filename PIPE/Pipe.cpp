#include "pipe.h"
PIPE_ PIPE[1110];

int clock_cnt, ended;

void pipe_init(const string &input) {
//    freopen(input.c_str(),"r",stdin);
    ifstream fin(input);
    int n = 0;
    while (fin >> hex >> PIPE[0].Datamemory.Val[n])
        n++;
    clock_cnt = ended = 0;
    PIPE[0].Init();
}

void proc_one() {
    char buf[1024];
    clock_cnt++;
    PIPE[clock_cnt] = PIPE[clock_cnt - 1];
    PIPE[clock_cnt].Proc(PIPE[clock_cnt - 1]);
    PIPE[clock_cnt].Print(buf);
    printf("%s\n", buf);
    ended = (PIPE[clock_cnt].stat != SAOK);
}
