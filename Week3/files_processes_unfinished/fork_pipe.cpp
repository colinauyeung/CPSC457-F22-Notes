#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string.h>

int main(){
    int fd[2];
    pipe(fd);

    pid_t id = fork();

    if(id == 0){ 

        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);


        char * args[] = {"ps", NULL};
        execvp("ps", args);

    }
    else{ 

        close(fd[1]);
        char buf;
        while(read(fd[0], &buf, 1) != 0){
            if(buf!='\n'){
                printf("%c-", buf);
            }
            else{
                printf("\n");
            }
        }
        close(fd[0]);

    }

    return 0;
}