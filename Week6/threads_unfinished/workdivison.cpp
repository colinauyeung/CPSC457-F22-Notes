#include <stdio.h>   
#include <pthread.h>  
#include <string> 
#include <unistd.h>   

int nums[] = {61, 69, 17, 21,52,18,12,15,65,52,45,40,0,21,44,34,83,83,
    11,77,62,36,1,29,43,22,9,33,39,94,8,54,85,86,26,14,41,9,38,95,38,
    77,88, 72, 76, 59, 64, 69, 12, 60};

int numsize = 50;

pthread_mutex_t lock;

struct input{
    int id;
    int start;
    int end;
    int nthread;
    int localresult = 0;
};

int result;

void * method1(void * arg){
    struct input * in = (struct input *) arg;
    int id = in->id;
    int nthread = in->nthread;
    int start = in->start;
    int end = in->end;
    int localresult = 0;
    for(int i = start; i<end; i=i+1){
        localresult = localresult + nums[i];
    }
    in->localresult = localresult;
    pthread_exit(NULL);

}

int main(){
    if(pthread_mutex_init(&lock, NULL) != 0){
        return 1;
    }

    int nthread = 4;
    int baseline = 0;
    for(int i=0;i<numsize; i++){
        baseline = baseline + nums[i];
    }
    printf("Serial Output: %d\n", baseline);

    pthread_t pool[nthread];
    struct input in[nthread];

    int div = numsize/nthread;
    int mod = numsize % nthread;

    int lastend = 0;
    for(int i = 0; i<nthread; i++){
        in[i].id = i;
        in[i].nthread = nthread; 
        in[i].start = lastend;
        if(i<mod){
            in[i].end = in[i].start + div+1;
        }
        else{
            in[i].end = in[i].start + div;
        }
        
        lastend = in[i].end;
        printf("Thread %d start %d end %d\n", i, in[i].start, in[i].end);
    }

    for(int i = 0; i<nthread; i++){
        pthread_create(&pool[i], NULL, method1, &in[i]);
    }

    for(int i = 0; i<nthread; i++){
        pthread_join(pool[i], NULL);
    }

    int result2 = 0;
    for(int i = 0; i<nthread; i++){
        result2 = result2 + in[i].localresult;
    }

    printf("Method 1: %d\n", result2);

    return 0;

}