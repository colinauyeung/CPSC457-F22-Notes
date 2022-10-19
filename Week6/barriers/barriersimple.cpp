#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <pthread.h>

struct data{
    int id;
};

pthread_barrier_t barrier;

void * thread_function(void * args){
    struct data *in_args = (struct data *) args;
    int id = in_args->id;

    printf("Multitreaded code: Thread %d\n", id);

    int flag = pthread_barrier_wait(&barrier);
    if(flag == PTHREAD_BARRIER_SERIAL_THREAD){
        printf("Single Threaded code: Thread %d\n", id);
    }

    pthread_barrier_wait(&barrier);

    printf("Multitreaded code: Thread %d\n", id);
    
    pthread_exit(NULL);
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

    
}