#include <stdio.h>   
#include <pthread.h>  
#include <string> 
#include <unistd.h>   

struct input{
    int id;
    char name;
};

void * threadcode(void * arg){
    struct input * in = (struct input *) arg;
    int id = in->id;
    char name = in->name;
    printf("Hello I'm thead %d, my name is %c\n", id, name);
    pthread_exit(NULL);
    return NULL;
}

int main(){
    int nthread = 4;
    pthread_t pool[nthread];
    struct input in[nthread];

    for(int i = 0; i<nthread; i++){
        in[i].id = i;
        in[i].name = 'a' + i; 
    }

    for(int i = 0; i<nthread; i++){
        pthread_create(&pool[i], NULL, threadcode, &in[i]);
    }

    for(int i = 0; i<nthread; i++){
        pthread_join(pool[i], NULL);
    }

    return 0;

}