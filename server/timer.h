

#ifndef TIMER_H
#define TIMEI_H

#include "pq.h"
#include "http_request.h"


#define TIMER_INFINITE -1
#define TIMEOUT_DEFAULT 500   /*ms*/


typedef int (*timer_hander_pt)(zv_http_request_t *rq);

typedef struct zv_timer_node_s{
    size_t key;
    int deleted;   /*if remote client close the socket first, set the delete to 1*/
    timer_hander_pt handler;
    zv_http_request_t* rq;
} zv_timer_node;

int zv_time_init();
int zv_finder_timer();
int zv_handle_expire_timers();

extern zv_pq_t zv_timer;
extern size_t zv_current_msec;

void zv_add_timer(zv_http_request_t* rq, size_t timeout, timer_handler_pt handler);
void zv_del_timer(zv_http_request_t* rq);



#endif    
