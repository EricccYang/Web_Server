
#include "epoll.h"


struct epoll_event* events;

int epoll_create(int flags){

    int fd = epoll_create1(flags);
    check(fd>0, "epoll_creat:");

    event = (struct epoll_event*)malloc(sizeof(struct epoll_event)*MAXEVENTS);
    check(events != NULL, "epoll_create: malloc");
    return fd;
}


void epoll_add(int epfd, int fd, struct epoll_event* event){
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

int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout){
    int n = epoll_wait(epfd, events, maxevents, timeout);
    check(n>=0,"epoll_wait");
    return n;
}