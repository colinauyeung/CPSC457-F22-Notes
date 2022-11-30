
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <atomic>

const int numsize = 500;
int nums[numsize];
int target = 225;
std::atomic<bool> done;

class printobj{
    private:
        pthread_mutex_t lock;
        bool enabled = false;

    public:
        printobj(){
            pthread_mutex_init(&lock, NULL);
        }

        void enable(){
            enabled = true;
        }

        void print(std::string s){
            if(enabled){
                pthread_mutex_lock(&lock);
                printf("%s\n", s.c_str());
                pthread_mutex_unlock(&lock);
            }
            
        }
};

struct arg{

    int id;
    int nthreads;
    printobj * p;
    int found = -1;

};

void * find(void * args){
    struct arg * in = (struct arg *) args;
    int id = in->id;
    int nthreads = in->nthreads;
    printobj printer = *(in->p);
    std::string res = "Hello I am " + std::to_string(id) + " I have looked at ";
    for(int i = id; i<numsize; i=i+nthreads){
        if(done.load(std::memory_order_relaxed)){
            break;
        }
       res = res + std::to_string(i) + " ";  
        if(nums[i]==target){
            done.store(true, std::memory_order_relaxed);
            in->found = i;
            break;
        }
    }
    printer.print(res);

    pthread_exit(0);
}


int main(){
    for(int i = 0; i<numsize; i++){
        nums[i] = i;
    }

    int nthreads = 4;
    done.store(false, std::memory_order_relaxed);

    printobj printer = printobj();
    printer.enable();

    struct arg args[nthreads];
    pthread_t pool[nthreads];

    for(int i = 0; i<nthreads; i++){
        args[i].id = i;
        args[i].nthreads = nthreads;
        args[i].p = &printer;
    }

    for(int i = 0; i<nthreads; i++){
        pthread_create(&pool[i], NULL, find, (void *) &args[i]);
    }

    for(int i = 0; i<nthreads; i++){
        pthread_join(pool[i], NULL);
    }


}