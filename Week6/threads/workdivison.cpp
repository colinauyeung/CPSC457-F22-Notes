#include <stdio.h>   
#include <pthread.h>  
#include <string> 
#include <unistd.h>   

/*

    In this file, we'll now actually get threads to do some work!
    Pretty simple program, we'll just have an array of numbers that we'll get our threads to sum up.

    For this, we'll walk through a few different ways of doing this (some better, some worse)

*/

//Here's our array and it's size for easy access 
int nums[] = {61, 69, 17, 21,52,18,12,15,65,52,45,40,0,21,44,34,83,83,
    11,77,62,36,1,29,43,22,9,33,39,94,8,54,85,86,26,14,41,9,38,95,38,
    77,88, 72, 76, 59, 64, 69, 12, 60};

int numsize = 50;

//I'm going to setup a mutex as well that we'll use in a few of different methods
pthread_mutex_t lock;

/*

    With any of ways that we want to tacke having the threads sum up the array (as well as more generally with threaded code),
    there's a number of key challenges that we'll need to figure out and make decisions about.
    
    1. How are we sub-dividing our task? How do we partition what we have such that we have distinct sections for all of our threads to do work on
    2. How are we combining and reporting results from our threads

    How we deal with these problems will affect how we decide how to structure our code

*/


/*

    Method 1: Bad version

    So refer to the main challenges above, the general idea of this code is to
    1. Use a round robin assignment to give work to our threads (this will be a static round robin, but should still nicely divid our work)
    2. Just use a global variable that all of our threads can access to report the result of thread

    Again this is a bad version of the code, but we'll see a little bit later a nicer way of doing this

*/

//First we need set up the struct for giving data to our threads
//For the round robin assignment from our array, all we need is a thread id and the total number of threads we have
struct input{
    int id;
    int nthreads;
};

//Next setup a global variable for the results
int result1 = 0;

//Basic thread set up
void * method1(void * args){

    //Type coercise the input and then save everything into local variables
    struct input * in = ((struct input *) args);
    int id = in -> id;
    int nthreads = in -> nthreads;

    //Now how do we loop such that we properly partition the work?
    //We need to make sure that the threads look at every element of our array and that no two threads end up looking at the same element

    //In this case, we'll do a varient of a round robin assignment using the id of a thread and the number of threads
    //The idea is that the 1st thread gets the 1st element, the second thread gets the 2nd element and so on
    //When we get to our last thread, we'll wrap around. So in this code, we'll have 4 threads, 
    //so the 4th thread will get the 4th element and the 1st thread will then get the 5th element
    //It should look something like this if we had 18 elements
    //  Thread 1:  1  5  9   13  17
    //  Thread 2:  2  6  10  14  18
    //  Thread 3:  3  7  11  15  
    //  Thread 4:  4  8  12  16 

    //How do we actually do this? Well notice that each thread starts on the element equal to it's id
    //and the difference is between each element that a thread gets is the number of threads we have!
    //So this becomes easy to set up in a for loop!
    for(int i = id; i<numsize; i=i+nthreads){

        //Okay, so we could just push our results directly to the global result
        //But that ends up being a bad idea. This basically just makes our code serial!
        //Because we're working with a shared variable, we really should protect it, so we'll use a mutex
        //But, if only one thread is allowed inside of our mutex at a time and this addition is the bulk of the work
        //we're doing, then this is basically the same as doing everything in one thread!
        pthread_mutex_lock(&lock);
        result1 = result1 + nums[i];
        pthread_mutex_unlock(&lock);
    }

    //Exiting
    pthread_exit(NULL);
    return NULL;
}

/*

    Method 1: Better Version

    This method is the same basic idea as the previous, but we'll fix our issue of our code 
    running basically like serial code by using a local copy of our results and combining them after

*/
void * method1better(void * args){
    struct input * in = ((struct input *) args);
    int id = in -> id;
    int nthreads = in -> nthreads;

    //So instead of directly working with global result, which we'll set up a local result
    //which we'll modify instead of the global variable
    int localres = 0;
    for(int i = id; i<numsize; i=i+nthreads){
        localres = localres + nums[i];
    }

    //And now at the end, we'll combine our result from this thread into the global result all at one
    //This means that we can do the bulk of our work in a nicely parallelized way before using a mutex
    //for the small section that needs to be protected
    pthread_mutex_lock(&lock);
    result1 = result1 + localres;
    pthread_mutex_unlock(&lock);


    pthread_exit(NULL);
    return NULL;
}

/*

    Method 1: Mutexless

    Okay, so depending on how result tight we are, there might be times where we want to minimize our mutex usage
    This version isn't nessasarily better than the previous, but there are times where the times from the mutex can be an issue
    This one is situationally better and slightly cleaner because of that.

*/

//So we're going to use a different input struct than before
//Remember that we pass these from the parent to the thread by reference!
//So when we modify the struct in our thread, it ends up getting modified for our parent!
//What we can do with this is to use the struct as a way to pass data from our threads to our parent!

//So here let's write a local result from our thread to our struct and then leave it to the parent to deal with
struct input2{
    int id;
    int nthreads;
    int localres;
};

//Same basic set up as before
void * method1other(void * args){
    struct input2 * in = ((struct input2 *) args);
    int id = in -> id;
    int nthreads = in -> nthreads;

    //We're still to do most of the work on a local variable version of our result as it saves a little bit of time
    //mostly because of optimizations that the compiler is doing.
    int localres = 0;
    for(int i = id; i<numsize; i=i+nthreads){
        localres = localres + nums[i];
    }

    //Now at the end, we'll write to the input struct for the parent to deal with
    in->localres =  localres;
    pthread_exit(NULL);
    return NULL;
}

/*

    Method 2:

    So now let's look at a totally different way of set dividing the work.
    While method 1's way of dividing work is perfectly fine for Assignment 3, there are times where having 
    contigious blocks of work for our threads is nicer

    On the thread side, we'll mostly rely on the parent to do the work of sub-dividing the work

    On the question of how to return our result, we'll just do the same as the previous

*/

//Notice in our input struct, we're just going to take in where we should start and stop as values
struct input3{
    int start;
    int end;
    int id;
    int localres;
};

//Same set up as before...
void * method2(void * args){
    struct input3 * in = ((struct input3 *) args);
    int start = in -> start; 
    int end = in -> end;
    int localres = 0;

    //Instead of doing any fancy tricks here, we'll just normally iterate from start to end and just rely on the 
    //parent to have giving us good start and end values
    for(int i = start; i<end; i++){
        localres = localres + nums[i];
    }
    in->localres = localres;

    //Exit...
    pthread_exit(NULL);
    return NULL;
}

//Finally the parent function...
int main(){

    /*
    
        Single Threaded Version of Our Code

        When doing threaded code, it's good to have a single threaded version to check against and make sure that
        you're doing everything right, so that's what I'll do here. 
    
    */

    int result = 0;
    for(int i = 0; i<numsize; i++){
        result = result + nums[i];
    }
    printf("Single Thread Result: %d \n", result);

    //
    pthread_mutex_init(&lock, NULL);
    

    int n = 4;

    /*
    
        Parent Driver Code for Method 1 Better

        (I'm just going to skip over the bad version, the code for the parent would be exactly the same)
    
    */

    pthread_t pool[n];
    input inputs[n];
    for(int i = 0; i<n; i++){
        inputs[i].id = i;
        inputs[i].nthreads = n;
    }
    for(int i = 0; i<n; i++){
        pthread_create(&pool[i], NULL, method1better, (void *) &inputs[i]);
    }
    for(int i = 0; i<n; i++){
        pthread_join(pool[i], NULL);    
    }
    printf("Method 1 Better Result: %d \n", result);

    /*
    
        Parent Driver Code for Method 1 Mutexless
    
    */

    int result2 = 0;
    pthread_t pool2[n];
    input2 inputs2[n];
    for(int i = 0; i<n; i++){
        inputs2[i].id = i;
        inputs2[i].nthreads = n;
        inputs2[i].localres = 0;
    }
    for(int i = 0; i<n; i++){
        pthread_create(&pool2[i], NULL, method1other, (void *) &inputs2[i]);
    }
    for(int i = 0; i<n; i++){
        pthread_join(pool2[i], NULL);    
    }
    for(int i = 0; i<n; i++){
        result2 = result2 + inputs2[i].localres;
    }
    printf("Method 1 Mutexless Result: %d \n", result2);

    /*
    
        Parent Driver Code for Method 2
    
    */

    input3 inputs3[n];
    int div = numsize / n;
    int mod = numsize % n;
    int lastend = 0;
    for(int i = 0; i<n; i++){
        inputs3[i].id = i;
        inputs3[i].localres = 0;
        inputs3[i].start = lastend;
        if(i < mod){
            inputs3[i].end = inputs3[i].start + div + 1;
        }
        else{
            inputs3[i].end = inputs3[i].start + div;
        }
        
        lastend = inputs3[i].end;
        // printf("Thread %d start: %d end: %d\n", inputs3[i].id, inputs3[i].start, inputs3[i].end );
    }
    int result3 = 0;
    pthread_t pool3[n];
    for(int i = 0; i<n; i++){
        pthread_create(&pool3[i], NULL, method2, (void *) &inputs3[i]);
    }
    for(int i = 0; i<n; i++){
        pthread_join(pool3[i], NULL);    
    }
    for(int i = 0; i<n; i++){
        result3 = result3 + inputs3[i].localres;
    }

    printf("Method 2 Result: %d \n", result3);
    
    // printf("Hello I'm back as the parent\n");
    pthread_mutex_destroy(&lock);

}