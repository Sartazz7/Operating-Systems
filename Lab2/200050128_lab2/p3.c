#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 52

void child(char fileName[]){
    system(fileName);
}

void parent(){
    printf(">>> ");
    char fileName[N];
    fgets(&fileName[2], N, stdin);
    fileName[0]='.';
    fileName[1]='/';
    int pid=fork();
    if(pid==0){
        child(fileName);
    }
    else {
        int status;
        waitpid(pid,&status,0);
        parent();
    }
}
  
int main(){
    parent();
}