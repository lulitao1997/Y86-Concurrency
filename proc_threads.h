//
// Created by nn on 2017/6/11.
//

#ifndef Y86_CONC_PROC_THREADS_H
#define Y86_CONC_PROC_THREADS_H

#include "PIPE/pipe.h"
#include "PIPE/transfer.h"
#include <thread>
#include <iostream>
#include <mutex>
using std::mutex;
using std::cerr;
mutex mtx, f_lock, d_lock, e_lock, m_lock, w_lock, fin_lock;
int fin_cnt, start_cnt;
PIPE_ *last_pipe_p;
const int ECHONUM = 5;
void serve_one(char *result, const char *uri);
void after_fork();
void before_wake_thread();
void proc_f();
void proc_d();
void proc_e();
void proc_m();
void proc_w();

void after_fork() {
    f_lock.lock();
    d_lock.lock();
    e_lock.lock();
    m_lock.lock();
    w_lock.lock();
    fin_lock.lock();
    thread f_t(proc_f);
    thread d_t(proc_d);
    thread e_t(proc_e);
    thread m_t(proc_m);
    thread w_t(proc_w);
    cout << "after fork complete" << endl;
}

void serve_one(char *result, const char *uri) {
    if (strcmp(uri, "/next") == 0) {
        if (!ended) {
            cerr << "receive POST!!" << endl;
            before_wake_thread();
            cerr << "begin to call proc threads" << endl;
            f_lock.unlock(); /*this wake those echo threads*/
            d_lock.unlock(); /*this wake those echo threads*/
            e_lock.unlock(); /*this wake those echo threads*/
            m_lock.unlock(); /*this wake those echo threads*/
            w_lock.unlock(); /*this wake those echo threads*/
            cerr << "unlocked!" << endl;
            fin_lock.lock(); /*wait echo threads to finish*/
            cerr << "threads finished" << endl;
            PIPE[clock_cnt].stat = PIPE[clock_cnt].W.stat;
        }
    }
    else if (strcmp(uri, "/prev") == 0) {
        if (clock_cnt) clock_cnt--;
    }
    else if (strcmp(uri, "/reset") == 0) {
        clock_cnt = 0;
    }
    cout << "==== CLOCK COUNT ====" << clock_cnt << endl;
    PIPE[clock_cnt].Print(result);
//        serve_json(connfd, result);
    ended = (PIPE[clock_cnt].stat != SAOK);
    fin_cnt = start_cnt = 0;
}

void before_wake_thread() {
    cerr << "befor wake" << endl;
    last_pipe_p = &PIPE[clock_cnt];
    PIPE[++clock_cnt] = *last_pipe_p;
    PIPE[clock_cnt].LHProc(*last_pipe_p);
}

void proc_f() {
    while (1) {
        f_lock.lock();
        mtx.lock();
        start_cnt++;
        cerr << "PROCING F" << start_cnt << fin_cnt << endl;
        mtx.unlock();


        PIPE[clock_cnt].ConveyF(*last_pipe_p);

        mtx.lock();
        fin_cnt++;
        cerr << "F FIN " << start_cnt << fin_cnt << endl;
        if (fin_cnt == ECHONUM)
            fin_lock.unlock(); //this wake the webserver thread.
        mtx.unlock();
    }
}
void proc_d() {
    while (1) {
        d_lock.lock();
        mtx.lock();
        start_cnt++;
        cerr << "PROCING D" << start_cnt << fin_cnt << endl;
        mtx.unlock();


        PIPE[clock_cnt].ConveyD(*last_pipe_p);

        mtx.lock();
        fin_cnt++;
        cerr << "D FIN " << start_cnt << fin_cnt << endl;
        if (fin_cnt == ECHONUM)
            fin_lock.unlock(); //this wake the webserver thread.
        mtx.unlock();
    }
}
void proc_e() {
    while (1) {
        e_lock.lock();
        mtx.lock();
        start_cnt++;
        cerr << "PROCING E" << start_cnt << fin_cnt << endl;
        mtx.unlock();


        PIPE[clock_cnt].ConveyE(*last_pipe_p);

        mtx.lock();
        fin_cnt++;
        cerr << "E FIN " << start_cnt << fin_cnt << endl;
        if (fin_cnt == ECHONUM)
            fin_lock.unlock(); //this wake the webserver thread.
        mtx.unlock();
    }
}
void proc_m() {
    while (1) {
        m_lock.lock();
        mtx.lock();
        start_cnt++;
        cerr << "PROCING M" << start_cnt << fin_cnt << endl;
        mtx.unlock();


        PIPE[clock_cnt].ConveyM(*last_pipe_p);

        mtx.lock();
        fin_cnt++;
        cerr << "M FIN " << start_cnt << fin_cnt << endl;
        if (fin_cnt == ECHONUM)
            fin_lock.unlock(); //this wake the webserver thread.
        mtx.unlock();
    }
}
void proc_w() {
    while (1) {
        w_lock.lock();
        mtx.lock();
        start_cnt++;
        cerr << "PROCING W" << start_cnt << fin_cnt << endl;
        mtx.unlock();


        PIPE[clock_cnt].ConveyW(*last_pipe_p);

        mtx.lock();
        fin_cnt++;
        cerr << "W FIN " << start_cnt << fin_cnt << endl;
        if (fin_cnt == ECHONUM)
            fin_lock.unlock(); //this wake the webserver thread.
        mtx.unlock();
    }
}

#endif //Y86_CONC_PROC_THREADS_H
