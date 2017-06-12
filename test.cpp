//
// Created by nn on 2017/6/8.
//

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <cstring>
//#include <unistd.h>
#include "csapp.h"
#include "proc_threads.h"
#include "webserver.h"

using namespace std;

void sigchld_handler(int sig) //line:conc:echoserverp:handlerstart
{
    while (waitpid(-1, 0, WNOHANG) > 0)
        ;
    return;
} //line:conc:echoserverp:handlerend

map<string, pid_t> name2pid;
map<string, int> name2wfd;
map<string, int> name2rfd;

void create_new(const char *hostname, int connfd) {
//    cerr << "new: " << hostname << endl;
    pid_t pid;
    int pipefd1[2], pipefd2[2];
    pipe(pipefd1);
    pipe(pipefd2);
    cout << "create: " << pipefd1[0]<<','<<pipefd1[1]<< ';' << pipefd2[0]<<','<<pipefd2[1] << endl;
    rio_t rio;
    char buf[MAXLINE], writebuf[MAXLINE];
    pid_t pa_pid = getpid();
    if ((pid=Fork()) == 0) {
        Close(connfd);
        //chlid
        Rio_readinitb(&rio, pipefd1[0]);
        int cnt = 0;

        after_fork();

        while (1) {
            int n;
            if ((n=Rio_readlineb(&rio, buf, MAXLINE))<=0)
                continue;
            cout << "child read:" << n << endl;

            cout << "get:" << buf << endl << "and doing sth..." << endl;
            serve_one(writebuf, buf);
//            sprintf(writebuf, "%d %s get: %s", cnt, hostname, buf);
            Rio_writen(pipefd2[1], writebuf, strlen(writebuf));
            cnt++;
        }
    }
    name2pid[hostname] = pid;
    name2wfd[hostname] = pipefd1[1];
    name2rfd[hostname] = pipefd2[0];
    cout << "parent complete" << endl;
}

sigset_t mask;

void proxy(char *result, const char *hostname, const char *input) {
//    cerr << "exist: " << hostname << endl;
    cout << "exist: readfd, writefd:" << name2rfd[hostname] << ',' << name2wfd[hostname]
         << endl << "input:" << input << endl;
    Rio_writen(name2wfd[hostname], (void*)input, strlen(input));
    cout << "write complete " << endl;
    rio_t rio;
    Rio_readinitb(&rio, name2rfd[hostname]);
    Rio_readlineb(&rio, result, MAXLINE);
    cout << "read complete" << "reslut: " << result << endl;
}

int main(int argc, char **argv) {
    Sigemptyset(&mask);

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <port> <filepath>\n", argv[0]);
        exit(0);
    }
    transfer(argv[2]);
    pipe_init("__ins_buf__");

    char hostname[MAXLINE], port[MAXLINE];
    Signal(SIGCHLD, sigchld_handler);
    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n, connfd=%d\n", hostname, port, connfd);

        if (!name2pid.count(hostname)) {
            create_new(hostname, connfd);
        }cout << "main get1stline compltet" << endl;
        rio_t rio;
        int n;
        char buf[MAXLINE], result[MAXLINE];
//        Rio_readinitb(&rio, connfd);
//        Rio_readlineb(&rio, buf, MAXLINE);
        char *tmp_p = get_first_line(connfd);
        if (tmp_p == NULL) {
            Close(connfd);
            continue;
        }



        char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
        sscanf(tmp_p, "%s %s %s", method, uri, version);
        uri[strlen(uri)] = '\n';
        proxy(result, hostname, uri);
        cout << "server produce:" << result << endl;
//        Rio_writen(connfd, result, MAXLINE);
        serve_json(connfd, result);
        Close(connfd);
//        cout << "one time complete" << endl << "=================" << endl;
    }

}