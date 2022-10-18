#include <stdio.h>   
#include <pthread.h>  
#include <unistd.h>   
#include <map>

pthread_mutex_t lock;
std::map<char,int> histogram;

struct args{
    long start;
    long end;
};



void * thread_function(void * a){
    struct args * in = (struct args *) a;
    long mystart = in->start;
    long myend = in->end;

    std::map<char,int> local_histogram;
    FILE * kinglear = fopen("kinglear.txt", "r");

    fseek(kinglear, mystart, SEEK_SET);
    char c;
    for(int i = 0; i+mystart<myend; i++){
        c = getc(kinglear);
        if(c!='\0'){
            local_histogram[c] = local_histogram[c] + 1;
        }
    }
    pthread_mutex_lock(&lock);
    // printf("=============Thread Histogram=============\n");
    // for(auto it = local_histogram.begin(); it!=local_histogram.end(); ++it){
    //     histogram[it->first] = histogram[it->first] + it->second;
    //     printf("%c=>%d\n", it->first, it->second);
    // }
    // printf("==========================================\n");
    pthread_mutex_unlock(&lock);
    fclose(kinglear);
    pthread_exit(NULL);
    return NULL;
}

int main(){
    if(pthread_mutex_init(&lock, NULL)!= 0){
        return 1;
    }

    FILE * kinglear = fopen("kinglear.txt", "r");
    fseek(kinglear, 0L, SEEK_END);
    long endpos = ftell(kinglear);
    fclose(kinglear);

    int n = 5;

    args inputs[n];

    long lastend = 0;
    long div = endpos / n;
    long mod = endpos % n;
    printf("end of file: %ld\n", endpos);
    printf("mod: %ld\n", mod);
    for(long i = 0; i<n; i++){
        inputs[i].start = lastend;
        if(i < mod){
            printf("Hello World\n");
            inputs[i].end = inputs[i].start + div + 1;
        }
        else{
            inputs[i].end = inputs[i].start + div;
        }
        // if(i == n-1){
        //     inputs[i].end = endpos;
        // }
        // else{
        //     inputs[i].end = inputs[i].start + div;
        // }
        lastend = inputs[i].end;
        printf("Thread %ld will have %ld to %ld\n", i, inputs[i].start, inputs[i].end);
    }

    pthread_t pool[n];

    for(int i = 0; i<n; i++){
        pthread_create(&pool[i], NULL, thread_function, (void *) &inputs[i]);
    }

    for(int i = 0; i<n; i++){
        pthread_join(pool[i], NULL);
    }
    for(auto it=histogram.begin(); it!=histogram.end(); ++it){
        printf("%c=>%d\n", it->first, it->second);

    }
    pthread_mutex_destroy(&lock);
    return 0;
}
