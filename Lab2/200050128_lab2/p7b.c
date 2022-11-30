#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

void process(int n){
    int childPID[n];
    for(int i=0; i<n; i++){
        int pid=fork();
        if(pid==0){
            printf("Child %d is created\n",getpid());
            exit(0);
        }
        else {
            childPID[i]=pid;
        }
    }
    for(int i=0; i<n; i++){
        int status;
        waitpid(childPID[i],&status,0);
    }
    for(int i=0; i<n; i++){
        printf("Child %d of parent %d exited\n",childPID[i],getpid());
    }
}

int main(){
    printf("Parent is : %d\n",getpid());
    printf("Number of children: ");
    int n;
    scanf("%d",&n);
    int pid=fork();
    if(pid==0){
        process(n);
    }
    else {
        int status;
        waitpid(pid,&status,0);
        printf("Parent exited\n");
    }
}