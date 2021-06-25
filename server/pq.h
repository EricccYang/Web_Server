
#ifndef PQ_H
#define PQ_H

#include <stdio.h>
#define PQ_DEFAULT_SIZE 100
#define  OK  0
#define ZV_OK 0


typedef int (*pq_comp_pt)(void* pi, void* pj);

typedef struct{
    void** pq;
    size_t nalloc;
    size_t size;
    pq_comp_pt comp;
}pq_t;

int pq_init(pq_t* pq_stru, pq_comp_pt comp, size_t size);

int pq_is_empty(pq_t* pq);

void* pq_min(pq_t* pq);

void* pq_max(pq_t* pq);

int pq_delim(pq_t* pq);

int pq_insert(pq_t* pq, void* item);

size_t pq_size(pq_t* pq);



int pq_sink(pq_t* pq_stru, size_t n);

#endif