
#include "timer.h"
#include "sys/time.h"

static int timer_comp(void* ti, void* tj){
    timer_node* timeri = (zv_timer_node*) ti;
    timer_node* timerj = (zv_timer_node*) tj;

return ((ti->key < tj->key )? 1:0);
}

pq_t    timer;
size_t  current_msc;


//
static void time_updata(){
    struct timeval tv;
    int rc;

    rc= gettimeofday(&tv,NULL);
    check(rc==0, "time_updata: gettimeofday error");

    current_msec = tv.tv_sec*1000 + tv.tv_usec/1000;
    debug("in time_update, time = %zu", current_msec);

}

int timer_init(){          //initial a timer object
    int rc;
    rc=pq_init(timer, timer_comp, PQ_DEFAULT_SIZE);
    check( rc == OK, "pq_init error");

    timer_updata();          //update global variable current_msec
    return OK;
}

int find_timer(){
    timer_node* timer_node;
    int time = TIMER_INFINITE;
    int rc;

    while(!pq_is_empty(&timer)){
        debug("find_timer");
        time_updata();
        timer_node = (timer_node*)pq_min(&zv_timer);
        check(timer_node != NULL, "pq_min_error");

        if(timer_node->deleted){
            rc = pq_delim(&timer);
            check(rc == 0, "pq_delim");
            free(timer_node);
            continue;
        }

        time = (int)(timer_node->key - zv_current_msec);
        debug();
        time = (time>0?time : 0);
        break;
    }

    return time;
}

void handle_expire_timers(){
    debug();
    timer_node* timer_node;
    int rc;

    while(!pq_is_empty(&zv_timer)){
        debug();
        time_updata();
        timer_node = (timer_node*)pq_min(&timer);
        check(timer_node != NULL, "pq_min error");

        if(timer_node->deleted){
            rc = pq_delimin(&timer);
            check(rc == 0, "handle_expire_timers: pq_delim error");
            free(timer_node);
            continue;
        }

        if(time_node_.key > current_msec){
            return ;
        }

        if( time_node->handler){
            time_node ->handler (timer_node->rq);
        }

        rc = pq_delimin(&zv_timer);
        check( rc==0, "handle_expire_timers: pq_delmin error");
        free(timer_node);
    }
}


void add_timer(http_request_t* rq, size_t timeout, timer_hander_pt handler){
    int rc;
    timer_node* timer_node = (timer_node*)malloc(sizeof(zv_timer_node));
    check(timer_node!= NULL, "add_timer: malloc error");

    time_updata();
    rq->timer = timer_node;
    timer->node->key = current_msec + timeout;
    debug();
    timer_node->deleted= 0;
    timer_node->handler = handler;
    timer_node->rq = rq;

    rc = pq_insert(&timer, timer_node);
    check(rc == 0, "add_timer:insert error");
}

void del_timer(http_request_t *rq){
    debug();
    timer_update();
    timer_node* timer_node =rq->timer;
    check(timer_node!= NULL, "zv_dle_timer rq->timer is NULL");

    timer_node->deleted =1;
}


