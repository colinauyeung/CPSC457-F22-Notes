
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <atomic>

const int numsize = 500;
int nums[numsize];
int target = 225;


struct arg{

    int id;
    int nthreads;


};

void * find(void * args){
    struct arg * in = (struct arg *) args;
    int id = in->id;
    int nthreads = in->nthreads;


    pthread_exit(0);
}


int main(){
    for(int i = 0; i<numsize; i++){
        nums[i] = i;
    }

    int nthreads = 4;


    struct arg args[nthreads];
    pthread_t pool[nthreads];

    for(int i = 0; i<nthreads; i++){
        args[i].id = i;
        args[i].nthreads = nthreads;
    }

    for(int i = 0; i<nthreads; i++){
        pthread_create(&pool[i], NULL, find, (void *) &args[i]);
    }

    for(int i = 0; i<nthreads; i++){
        pthread_join(pool[i], NULL);
    }


}