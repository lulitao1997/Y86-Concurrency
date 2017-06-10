//
// Created by nn on 2017/6/8.
//

#ifndef Y86_CONC_WEBSERVER_H
#define Y86_CONC_WEBSERVER_H
/* Network helper functions */
#include "csapp.h"
void doit(int fd, char *method, char *uri, char *version);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd,const char *filename, size_t filesize);
void get_filetype(const char *filename, char *filetype);
void serve_dynamic(int fd,const char *filename,const char *cgiargs);
void clienterror(int fd,const char *cause,const char *errnum,
                 const char *shortmsg,const char *longmsg);
char *get_first_line(int fd);
void serve_json(int fd, const char *json);
#endif //Y86_CONC_WEBSERVER_H
