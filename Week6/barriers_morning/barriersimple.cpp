#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <pthread.h>

const int nthread = 4;
pthread_barrier_t bar;

struct data{
    int id;
};


void * thread_function(void * args){
    struct data *in_args = (struct data *) args;
    int id = in_args->id;
    printf("Multithread code. ID: %d\n", id);
    int flag = pthread_barrier_wait(&bar);
    if(flag == PTHREAD_BARRIER_SERIAL_THREAD){
        printf("Serial code. ID:%d\n", id);
    }
    pthread_barrier_wait(&bar);
    printf("Multithread code. ID: %d\n", id);
    pthread_exit(NULL);
}

int main(){
    pthread_barrier_init(&bar, NULL, nthread);

    pthread_t pool[nthread];
    struct data args[nthread];

    for(int i = 0; i<nthread; i++){
        args[i].id = i;
    }
    
    for(int i = 0; i<nthread; i++){
        pthread_create(&pool[i], NULL, thread_function, (void *) &args[i]);
    }
    
    for(int i=0; i<nthread; i++){
        pthread_join(pool[i], NULL);
    }

    
}