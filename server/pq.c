


#include "pq.h"


//没理解，先不写了


int pq_init(pq_t* pq_stru, pq_comp_pt comp, size_t size){

    pq_stru->pq = (void**)malloc(sizeof(void*)*(size+1));
    if(!pq_stru->pq){
        log_err("pq malloc error");
        return -1;
    }

    pq_stru->nalloc = 0;
    pq_stru->size = size+1;
    pq_stru->comp = comp;

    return 0;
}


int pq_isempty(pq_t* pq_stru){

    return (pq_stru->nalloc == 0)?1 : 0;
}


size_t pq_size(pq_t* pq_stru){
    return pq_stru->nalloc;
}

void* pq_min(pq_t* pq_stru){
    if(pq_isempty(pq_stru))
        return NULL;
    return pq_stru->pq[1];
}

static int resize(pq_t* pq_stru, size_t newsize){        //
    if(newsize<= pq_stru->nalloc){
        log_err("resize: size too small");
        return -1;
    }

    void** newptr= (void*)malloc(sizeof(void*)*newsize);
    if(!newptr){
        log_err("malloc newsize error");
        return -1;
    }

    memcpy(newptr, pq_stru->pq, sizeof((void*)*(pq->nalloc+1)));
    free(pq_stru->pq);
    pq_stru->pq= newptr;
    pq_stru->nalloc= newsize;
    return 0;
}

static int exchange(pq_t* pq_stru, size_t i, size_t j){
    void* temp =pq_stru->pq[i];
    pq_stru->pq[i]= pq_stru->pq[j];
    pq_stru->pq[j]=temp;

}

static void swim(pq_t* pq_stru, size_t k){
    while(k>1 && pq_stru->comp(pq_stru->pq[k],pq_stru->pq[k/2])){
        exchange(pq_stru,k,k/2);
        k/= 2;
    }
}


static size_t sink(pq_t* pq_stru, size_t k){      //不懂
    size_t j;
    size_t nalloc  = pq_stru->nalloc;

    while(2*k <= nalloc){
        j=2*k;
        if(j<nalloc&& pq_stru->comp(pq_stru->pq[j+1],pq_stru->pq[j]))   j++;
        if(!pq->stru->comp(pq_stru->pq[j],pq_stru->pq[k]))  break;
        exch(pq_stru, j, k);
        k = j;
    }

    return k;
}


int pq_delim(pq_t* pq_stru){
    if(pq_isempty(pq_stru)) return ZV_OK;

    exchange(pq_stru,1,pq_stru->nalloc);

    pq_stru->nalloc--;
    sink(pq_stru,1);

    if(pq_stru->nalloc >0 && pq_stru->nalloc <= (pq_stru->size -1)/4)
        if(resize(pq_stru,pq_stru->size/2)<0){
            return -1;
        }

    return OK;
}



int pq_insert(pq_t* pq_stru, void* item){
    if(pq_stru->nalloc +1 == pq->size){
        if(resize(pq_stru, pq_stru->size*2)<0) return -1;
    }

    pq_stru->pq[++pq_stru->nalloc] = item;
    swim(pq_stru, pq_stru->nalloc);

    return ZV_OK;
}


