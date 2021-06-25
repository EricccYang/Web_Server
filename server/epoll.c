
#include "epoll.h"
#include "debug.h"

#include "stdio.h"


extern struct epoll_event* events;

int epoll_create(int flags){

    int fd = epoll_create1(flags);
    check(fd>0, "epoll_creat:");

    struct epoll_event* event = (struct epoll_event*)malloc(sizeof(struct epoll_event)*MAXEVENTS);
    check(event != NULL, "epoll_create: malloc");
    return fd;
}


int epoll_add(int epfd, int fd, struct epoll_event* event){
    int rc = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, event);
    check(rc== 0, "epoll_add");
    return fd;
}

void epoll_mod(int epfd, int fd, struct epoll_event* event){
    int rc = epoll_ctl(epfd, EPOLL_CTL_MOD,fd, event);
    check(rc == 0, "epoll_mod");
    return;
}

void epoll_del(int epfd, int fd, struct epoll_event* event){
    int rc = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, event);
    check(rc == 0, "epoll_delt");
    return;
}

int epoll_wait_wrap(int epfd, struct epoll_event* events, int maxevents, int timeout){
    int n = epoll_wait(epfd, events, maxevents, timeout);
    int err = errno;
    int b =0 ;

    check(n >= 0, "epoll_wait");
    return n;
}