#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <pthread.h>
#include<iostream>

const int nthreads = 4;
const int power = 9;

int nums[] = {1,2,3,4,5,6,7,8,9};
int sizenums = 9;
int localres[nthreads];
int results[9];
int index = 0;
bool done = false;

struct data{
    int id;
};

pthread_barrier_t barrier;

void * thread_function(void * args){

    struct data *in_args = (struct data *) args;
    int id = in_args->id;

    int flag = pthread_barrier_wait(&barrier);
    if(flag == PTHREAD_BARRIER_SERIAL_THREAD){
        index = 0; 
        done = false;
    }
    pthread_barrier_wait(&barrier);
    while(1){
        if(done){
            pthread_exit(NULL);
        }
        int res = 1; 
        int div = power /nthreads;
        int mod = power % nthreads;
        for(int i = 0; i<div; i++){
            res = res * nums[index];
        }
        if(id<mod){
            res = res * nums[index];
        }
        localres[id] = res;
        flag = pthread_barrier_wait(&barrier);
        if(flag == PTHREAD_BARRIER_SERIAL_THREAD){
            int finalres = 1;
            for(int i = 0; i<nthreads; i++){
                finalres = finalres * localres[i];
            }
            results[index] = finalres;

            index++;
            if(index>=sizenums){
                done = true;
            }
        }
        pthread_barrier_wait(&barrier);
    }

    
    

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

    for(int i=0; i<sizenums; i++){
        printf("Results of %d^%d is %d\n", nums[i], power, results[i]);
    }
    return 0;


}