
#include "timer.h"
#include "debug.h"

#include "sys/time.h"

static int timer_comp(void* ti, void* tj){
    timer_node* timeri = (timer_node*) ti;
    timer_node* timerj = (timer_node*) tj;

return ((timeri->key < timerj->key )? 1:0);
}

pq_t    timer;
size_t  current_msec;


//
static void time_update(){
    struct timeval tv;
    int rc;

    rc= gettimeofday(&tv,NULL);
    check(rc==0, "time_updata: gettimeofday error");

    current_msec = tv.tv_sec*1000 + tv.tv_usec/1000;
    debug("in time_update, time = %zu", current_msec);

}

int timer_init(){          //initial a timer object
    int rc;
    rc=pq_init(&timer, timer_comp, PQ_DEFAULT_SIZE);
    check( rc == OK, "pq_init error");

    time_update();          //update global variable current_msec
    return OK;
}

int find_timer(){
    timer_node* node;
    int time = TIMER_INFINITE;
    int rc;

    while( !pq_is_empty(&timer) ){
        debug("find_timer");
        time_update();
        node = pq_min(&timer);
        check(node != NULL, "pq_min_error");

        if(node->deleted){
            rc = pq_delim(&timer);
            check(rc == 0, "pq_delim");
            log_info("%s %d", "attempt to free", node);
            free(node);
            continue;
        }

        time = (int)(node->key - current_msec);
        debug();
        time = (time > 0 ? time : 0);
        break;
    }

    return time;
}

void handle_expire_timers(){
    debug();
    timer_node* node;
    int rc;

    while(!pq_is_empty(&timer)){
        debug();
        time_update();
        node = pq_min(&timer);
        check(node != NULL, "pq_min error");

        if(node->deleted){
            rc = pq_delim(&timer);
            check(rc == 0, "handle_expire_timers: pq_delim error");
          log_info("%s %d", "attempt to free", node);
          free(node);
            continue;
        }

        if(node->key > current_msec){
            return ;
        }

        if(node->handler){
            node->handler(node->rq);
        }

        rc = pq_delim(&timer);
        check( rc==0, "handle_expire_timers: pq_delmin error");
      log_info("%s %d", "attempt to free", node);
      free(node);
    }
}


void add_timer(http_request_t* rq, size_t timeout, timer_handler_pt handler){
    int rc;
    timer_node* node =  malloc(sizeof(timer_node));
    check(node!= NULL, "add_timer: malloc error");

    time_update();
    rq->timer = node;
    node->key = current_msec + timeout;
    debug();
    node->deleted= 0;
    node->handler = handler;
    node->rq = rq;

    rc = pq_insert(&timer, node);
    check(rc == 0, "add_timer:insert error");
}

void del_timer(http_request_t *rq){
    debug();
    time_update();
    timer_node* node =rq->timer;
    check(node!= NULL, "zv_dle_timer rq->timer is NULL");

    node->deleted =1;
}


