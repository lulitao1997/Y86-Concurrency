/*-- Interacts with javascripts and EXEs of five stages --*/

#include "csapp.h"
#include <cstdio>
#include <iostream>
#include "consts.h"
#include "webserver.h"
#include "map_io.h"
#include "PIPE/pipe.h"
#include "PIPE/transfer.h"
#include "proc_threads.h"
#include <thread>
#include <mutex>
#include <map>

const int NTHREADS = 4;
const int SBUFSIZE = 16;

using namespace std;

/* customized server routine */
void webserver(int pt_n);
/* Thread routine */

//bool f_fin, d_fin, e_fin, m_fin, w_fin;

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
    pipe_init("__ins_buf__", 0);
    pipe_init("__ins_buf__", 1000);
    pipe_init("__ins_buf__", 2000);
    pipe_init("__ins_buf__", 3000);
    cerr << "write complete" << endl;
    f_lock.lock();
    d_lock.lock();
    e_lock.lock();
    m_lock.lock();
    w_lock.lock();
    fin_lock.lock();

//    webserver(webserv_port);

//    thread web_t(webserver, webserv_port);
    thread f_t(proc_f);
    thread d_t(proc_d);
    thread e_t(proc_e);
    thread m_t(proc_m);
    thread w_t(proc_w);
//    web_t.join();
    webserver(webserv_port);
    f_t.join();
    d_t.join();
    e_t.join();
    m_t.join();
    w_t.join();
}

int cnt4process[4] = {0, 1000, 2000, 3000};
int pcnt;
map<string, int> name2pid;

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

        if (!name2pid.count(hostname)) {
            name2pid[hostname] = pcnt++;
        }
        clock_cnt = cnt4process[name2pid[hostname]];
        if (strcasecmp(method, "GET") == 0) {
            if (strcmp(uri, "/next") == 0) {
                if (!ended) {
                    cerr << "receive POST!!" << endl;
                    before_wake_thread();
                    cnt4process[name2pid[hostname]]++;
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
                if (clock_cnt % 1000) cnt4process[name2pid[hostname]]--;
            }
            else if (strcmp(uri, "/reset") == 0) {
                cnt4process[name2pid[hostname]] = 1000*name2pid[hostname];
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
