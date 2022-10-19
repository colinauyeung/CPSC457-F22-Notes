#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <pthread.h>
#include<iostream>

const int nthreads = 4;
const int power = 9;

int nums[] = {1,2,3,4,5,6,7,8,9};
int sizenums = 9;

struct data{
    int id;
};

pthread_barrier_t barrier;

void * thread_function(void * args){

    struct data *in_args = (struct data *) args;
    int id = in_args->id;

    pthread_exit(NULL);

}

int main(){
    pthread_barrier_init(&barrier, NULL , nthreads);
    pthread_t pool[nthreads];
    struct data args[nthreads];

    for(int i = 0; i<nthreads; i++){
        args[i].id = i;
    }
    
    for(int i = 0; i<nthreads; i++){
        pthread_create(&pool[i], NULL, thread_function, (void *) &args[i]);
    }
    
    for(int i=0; i<nthreads; i++){
        pthread_join(pool[i], NULL);
    }


}