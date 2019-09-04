


#include "timer.h"
#include "sys/time.h"

static int timer_comp(void* ti, void* tj){
    zv_timer_node* timeri = (zv_timer_node*) ti;
    zv_timer_node* timerj = (zv_timer_node*) tj;

return ((ti->key < tj->key )? 1:0);
}

pq_t zv_timer;
size_t  current_msc;


//
static void zv_time_updata(){
    struct timeval tv;
    int rc;

    rc= gettimeofday(&tv,NULL);
    check(rc==0, "zv_time_updata: gettimeofday error");

    current_msec = tv.tv_sec*1000 + tv.tv_usec/1000;
    debug("in zv_time_update, time = %zu", zv_current_msec);

}

int zv_timer_init(){          //initial a timer object
    int rc;
    rc=zv_pq_init(zv_timer, timer_comp, ZV_PQ_DEFAULT_SIZE);
    check( rc == ZV_OK, "zv_pq_init error");

    zv_timer_updata();          //update global variable current_msec
    return ZV_OK;
}

int zv_find_timer(){
    zv_timer_node* timer_node;
    int time = ZV_TIMER_INFINITE;
    int rc;

    while(!zv_pq_is_empty(&zv_timer)){
        debug("zv_find_timer");
        zv_time_updata();
        timer_node = (zv_timer_node*)zv_pq_min(&zv_timer);
        check(timer_node != NULL, "zv_pq_min_error");

        if(timer_node->deleted){
            rc = zv_pq_delim(&zv_timer);
            check(rc == 0, "zv_pq_delim");
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

void zv_handle_expire_timers(){
    debug();
    zv_timer_node* timer_node;
    int rc;

    while(!zv_pq_is_empty(&zv_timer)){
        debug();
        zv_time_updata();
        timer_node = (zv_timer_node*)zv_pq_min(&zv_timer);
        check(timer_node != NULL, "zv_pq_min error");

        if(timer_node->deleted){
            rc = zv_pq_delimin(&zv_timer);
            check(rc == 0, "zv_handle_expire_timers: zv_pq_delim error");
            free(timer_node);
            continue;
        }

        if(time_node_.key > zv_current_msec){
            return ;
        }

        if( time_node->handler){
            time_node ->handler (timer_node->rq);
        }

        rc = zv_pq_delimin(&zv_timer);
        check( rc==0, "zv_handle_expire_timers: zv_pq_delmin error");
        free(timer_node);
    }
}


void zv_add_timer(zv_http_request_t* rq, size_t timeout, timer_hander_pt handler){
    int rc;
    zv_timer_node* timer_node = (zv_timer_node*)malloc(sizeof(zv_timer_node));
    check(timer_node!= NULL, "add_timer: malloc error");

    zv_time_updata();
    rq->timer = timer_node;
    timer->node->key = zv_current_msec + timeout;
    debug();
    timer_node->deleted= 0;
    timer_node->handler = handler;
    timer_node->rq = rq;

    rc = zv_pq_insert(&zv_timer, timer_node);
    check(rc == 0, "zv_add_timer:insert error");
}

void zv_del_timer(zv_http_request_t *rq){
    debug();
    zv_timer_update();
    zv_timer_node* timer_node =rq->timer;
    check(timer_node!= NULL, "zv_dle_timer rq->timer is NuLL");

    timer_node->deleted =1;
}


