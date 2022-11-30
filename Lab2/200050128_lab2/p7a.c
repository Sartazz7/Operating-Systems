#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

void childProcess(int n){
    if(n==0)return;
    printf("Child %d is created\n",getpid());
    int pid=fork();
    if(pid==0){
        childProcess(n-1);
    }
    else {
        int status;
        waitpid(pid,&status,0);
        printf("Child %d of parent %d exited\n",getpid(),getppid());
    }
}

void parentProcess(){
    printf("Parent is : %d\n",getpid());
    int n;
    printf("Number of children: ");
    scanf("%d",&n);
    int pid=fork();
    if(pid==0){
        childProcess(n);
    }
    else{
        int status;
        waitpid(pid,&status,0);
        printf("Parent exited\n");
    }
}

int main(){
    parentProcess();
}