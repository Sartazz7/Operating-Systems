#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(){
    int pid=fork();
    if (pid == 0){
        printf("Child : My process ID is: %d\n", getpid());
        printf("Child : The parent process ID is: %d\n", getppid());
        sleep(5);
        printf("\nAfter sleeping for 5 seconds\n");
        printf("Child : My process ID is: %d\n", getpid());
        printf("Child : The parent process ID is: %d\n", getppid());
    }
    else {
        printf("Parent : My process ID is: %d\n", getpid());
        printf("Parent : The child process ID is: %d\n", pid);
    }
    return 0;
}