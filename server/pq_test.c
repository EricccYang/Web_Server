//
// Created by kaicheng yang on 2021/6/29.
//

#include "stdlib.h"
#include "stdio.h"

typedef int (*comm)(int i, int j);


struct pq {

  int* t;
  int size;
  int capacity;
  comm com;
};
struct pq q;


int comp_stru(int i , int j){

  return i < j ? 1 : 0;

}

int is_empty(){
  return q.size == 0 ? 1 : 0;
}

void exchange(int i, int j){
  int tmp = q.t[j];
  q.t[j] = q.t[i];
  q.t[i] = tmp;
}

//insert
void insert(int new){
  q.t[++q.size] = new;
  int index = q.size/2;
  while(index > 0 && new < q.t[index]){
    exchange(q.size, index);
    index = index/2;
  }
}

int min(){
  if(!is_empty()){
    return q.t[1];
  }
  return -1;
}

void sink(int i){
  int index = 2*i;
  while(index < q.size){
    if(q.t[index] > q.t[index+1]) index++;
    if (q.t[i] > q.t[index]) {
      exchange(i, index);
    } else {
      break;
    }
    index = 2* index;
  }
}

void del(){
  exchange(1, q.size);
  q.size--;
  sink(1);
}



int main(){


  q.com = comp_stru;
  q.size = 0;
  q.capacity = 101;

  q.t = (int*)malloc(sizeof(int)*q.capacity);

  insert( 1);
  printf("%d\n", q.size);
  insert(2);
  insert(3);
  insert(9);
  printf("%d\n", q.size);



  for(int i = 1 ; i<= q.size; i++){
    printf("%d\n", q.t[i]);
  }

  del();
  for(int i = 1 ; i<= q.size; i++){
    printf("%d\n", q.t[i]);
  }


}