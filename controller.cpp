/*-- Interacts with javascripts and EXEs of five stages --*/

#include "csapp.h"
#include <cstdio>
#include <iostream>
#include "consts.h"
#include "webserver.h"
#include "map_io.h"
#include "PIPE/pipe.h"
#include "PIPE/transfer.h"

#include <thread>
#include <mutex>
#include <map>

const int NTHREADS = 4;
const int SBUFSIZE = 16;
const int ECHONUM = 5;

using namespace std;

/* customized server routine */
void webserver(int pt_n);
/* Thread routine */
void echoserver(int port_n);

mutex mtx, f_lock, d_lock, e_lock, m_lock, w_lock, fin_lock;
int fin_cnt = 0, start_cnt;

map<string, string> var;

//bool f_fin, d_fin, e_fin, m_fin, w_fin;

PIPE_ *last_pipe_p;
void before_wake_thread();
void proc_f();
void proc_d();
void proc_e();
void proc_m();
void proc_w();

int main(int argc, char **argv)
{
    prt_var(webserv_port);
    prt_var(fetch_port);
    prt_var(decode_port);
    prt_var(execute_port);
    prt_var(memory_port);
    prt_var(writeback_port);


    if (argc != 2) {
        printf("usage: %s <filepath>\n", argv[0]);
        exit(1);
    }

    transfer(argv[1]);
    pipe_init("__ins_buf__");
    cerr << "write complete" << endl;
    f_lock.lock();
    d_lock.lock();
    e_lock.lock();
    m_lock.lock();
    w_lock.lock();
    fin_lock.lock();

//    webserver(webserv_port);

    thread web_t(webserver, webserv_port);
    thread f_t(proc_f);
    thread d_t(proc_d);
    thread e_t(proc_e);
    thread m_t(proc_m);
    thread w_t(proc_w);
    web_t.join();
    f_t.join();
    d_t.join();
    e_t.join();
    m_t.join();
    w_t.join();
}

void web_process() {

}

map<string, pid_t> host2pid;

pid_t create_process(const char *host) {

}

void webserver(int pt_n) {

    int listenfd, connfd;
    char pt[MAXLINE], hostname[MAXLINE], port[MAXLINE];
    sprintf(pt, "%d", pt_n);
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;


    cerr << "FUCK THERE" << pt <<':'<<strlen(pt)<< endl;
    listenfd = Open_listenfd(pt);

    char method[MAXLINE], uri[MAXLINE], version[MAXLINE], result[MAXLINE];
    ended = false;

    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        /*
         * if signal "STEP" received:
         *   w_lock.unlock()
         *   wait finish_lock
         *   get var value from global map
         *   write msg;
         * else:
         *   write msg;
         */
        char *tmp_p = get_first_line(connfd);
        if (tmp_p == NULL) {
            Close(connfd);
            continue;
        }
        sscanf(tmp_p, "%s %s %s", method, uri, version);
        cerr << "FUCK THERE" << endl;
        if (strcasecmp(method, "GET") == 0) {
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
            serve_json(connfd, result);
            ended = (PIPE[clock_cnt].stat != SAOK);
            fin_cnt = start_cnt = 0;
        }
        else {
            doit(connfd, method, uri, version);
        }                                             //line:netp:tiny:doit
        Close(connfd);                                            //line:netp:tiny:close
    }
}

void echoserver(int port_n) {
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char port[MAXLINE], client_hostname[MAXLINE], client_port[MAXLINE];
    sprintf(port, "%d", port_n);
    listenfd = Open_listenfd(port);

    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
    printf("Connected to (%s, %s)\n", client_hostname, client_port);
//        echo(connfd);
    // do sth.
    rio_t rio;
    char buf[MAXLINE];
    ssize_t n;

    Rio_readinitb(&rio, connfd);
    while (1) {
        mtx.lock();
        w_lock.lock();
        mtx.unlock();

        n = Rio_readlineb(&rio, buf, MAXLINE);
        if (n == 0) break;
        printf("server received %d bytes\n", (int)n);
        map_read(var, buf); /*update the var*/

        mtx.lock();
        fin_cnt++;
        if (fin_cnt == ECHONUM)
            fin_lock.unlock(); //this wake the webserver thread.
        mtx.unlock();
    }

    Close(connfd);

}
//void proc_one() {
//    clock_cnt++;
//    PIPE[clock_cnt] = PIPE[clock_cnt - 1];
//    PIPE[clock_cnt].Proc(PIPE[clock_cnt - 1]);
//    PIPE[clock_cnt].Print();
//    printf("\n");
//    ended = (PIPE[clock_cnt].stat != SAOK);
//}

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