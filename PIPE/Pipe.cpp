#include "pipe.h"
PIPE_ PIPE[1110];

int clock_cnt, ended;

void pipe_init(const string &input) {
    freopen(input.c_str(),"r",stdin);
    int n = 0;
    while (cin >> hex >> PIPE[0].Datamemory.Val[n])
        n++;
    clock_cnt = ended = 0;
    PIPE[0].Init();
}

void proc_one() {
    clock_cnt++;
    PIPE[clock_cnt] = PIPE[clock_cnt - 1];
    PIPE[clock_cnt].Proc(PIPE[clock_cnt - 1]);
//    PIPE[clock_cnt].Print(2);
    printf("\n");
    ended = (PIPE[clock_cnt].stat != SAOK);
}
