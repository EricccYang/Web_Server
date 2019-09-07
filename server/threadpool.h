

#ifndef THREADPOOL_H
#define THREADPOOL_H


#ifndef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

#include"dbg.h"

#define THREAD_NUM 8

typedef struct zv_task_s {

    void (*func)(void *);

    void *arg;

    struct zv_task_s *next;

} zv_task_t;


typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t *threads;
    zv_task_t *head;
    int thread_count;
    int queue_size;
    int shutdown;
    int started;
} zv_threadpool_t;


typedef enum {
    threadpool_invalid = -1,
    threadpool_lock_fail = -2,
    threadpool_already_shutdown = -3,
    threadpool_cond_broadcast = -4,
    threadpppl_thread_fail = -5,
} threadpool_error_t;


threadpool_t *threadpool_init(int thread_num);

int threadpool_add(threadpool_t *pool, void(*func)(void *), void *arg);

int threadpool_destroy(threadpool_t *pool, int graceful);

#ifdef __cplusplus
}
#endif

#endif




