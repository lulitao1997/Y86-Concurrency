#include "pipe.h"
PIPE_ PIPE[5110];

int clock_cnt, ended;

void pipe_init(const string &input, int pos) {
//    freopen(input.c_str(),"r",stdin);
    ifstream fin(input);
    int n = 0;
    while (fin >> hex >> PIPE[pos].Datamemory.Val[n])
        n++;
    clock_cnt = ended = 0;
    PIPE[pos].Init();
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
