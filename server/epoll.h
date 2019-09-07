


#ifndef EPOLL_H
#define EPOLL_H


#include <sys/epoll.h>

#define MAXEVENTS 1024

int epoll_create(int flags);
int epoll_add(int epfd, int fd, struct epoll_event* event);
void epoll_mod(int epfd, int fd, struct epoll_event* event);
void epoll_del(int epfd, int fd, struct epoll_event* event);
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);


#endif