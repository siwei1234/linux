#ifndef _SYSUTIL_H_
#define _SYSUTIL_H_

#include"common.h"
void getloalip(char* ip);
int tcp_server(const char* host, unsigned short port);
int tcp_client(int port);
const char* statbuf_get_perms(struct stat *sbuf);
const char* statbuf_get_date(struct stat *sbuf);
void send_fd(int sock_fd, int fd);
int recv_fd(const int sock_fd);
long get_time_sec();
long get_time_usec();
void nano_sleep(double sleep_time);
#endif /* _SYSUTIL_H_ */