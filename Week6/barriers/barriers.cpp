#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <pthread.h>
#include<iostream>

const int nthreads = 4;
const int power = 9;

int nums[] = {1,2,3,4,5,6,7,8,9};
int sizenums = 9;

int currentindex = 0;
int64_t localresults[nthreads];
int64_t finalresults[9];
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
        
        currentindex = 0;
        done = false;

    }
    pthread_barrier_wait(&barrier);

    while(1){

        if(done){
            pthread_exit(NULL);
        }

        int64_t res = 1;
        int times = power/nthreads;
        if(id < (power % nthreads)){
            times = times + 1;
        }
        for(int i = 0; i<times; i++){
            res = res * nums[currentindex];
        }

        localresults[id] = res;

        flag = pthread_barrier_wait(&barrier);
        if(flag == PTHREAD_BARRIER_SERIAL_THREAD){
            
            int serialres = 1;
            for(int i = 0; i<nthreads; i++){
                serialres = serialres * localresults[i];
            }

            finalresults[currentindex] = serialres;
            currentindex++; 
            if(currentindex >= sizenums){
                done = true;
            }

        }
        pthread_barrier_wait(&barrier);
    }
}

int main(){
    pthread_barrier_init(&barrier, NULL ,4);
    pthread_t pool[4];
    struct data args[4];

    for(int i = 0; i<4; i++){
        args[i].id = i;
    }
    
    for(int i = 0; i<4; i++){
        pthread_create(&pool[i], NULL, thread_function, (void *) &args[i]);
    }
    
    for(int i=0; i<4; i++){
        pthread_join(pool[i], NULL);
    }

    for(auto a: finalresults){
        std::cout << a << std::endl;
    }


}