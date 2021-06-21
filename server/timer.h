

#ifndef TIMER_H
#define TIMEI_H

#include "pq.h"
#include "http_request.h"


#define TIMER_INFINITE -1
#define TIMEOUT_DEFAULT 500   /*ms*/


typedef int (*timer_hander_pt)(http_request_t *rq);

typedef struct timer_node_s{
    size_t key;
    int deleted;   /*if remote client close the socket first, set the delete to 1*/
    timer_hander_pt handler;
    http_request_t* rq;
} timer_node;

int timer_init();
int find_timer();
int handle_expire_timers();

extern pq_t timer;
extern size_t current_msec;

void add_timer(http_request_t* rq, size_t timeout, timer_handler_pt handler);
void del_timer(http_request_t* rq);



#endif    
