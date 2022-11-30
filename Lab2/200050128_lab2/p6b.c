#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(){
    int pid=fork();
    if (pid == 0){
        printf("Child : My process ID is: %d\n", getpid());
        printf("Child : The parent process ID is: %d\n", getppid());
        printf("\nChild : Check child process state\n");
        printf("\nChild : Press any key to continue\n");
        char c;
        scanf("%c",&c);
        printf("\nChild : Check child process state again\n");
    }
    else {
        printf("Parent : My process ID is: %d\n", getpid());
        printf("Parent : The child process ID is: %d\n", pid);
        sleep(60);
    }
    return 0;
}