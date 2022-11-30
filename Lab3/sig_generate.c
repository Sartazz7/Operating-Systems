#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int pid = atoi(argv[1]);
    kill(pid,SIGINT);
    printf("SIGINT signal sent to PID: %d\n",pid);
    kill(pid,SIGTERM);
    printf("SIGTERM signal sent to PID: %d\n",pid);
    kill(pid,SIGKILL);
    printf("SIGKILL signal sent to PID: %d\n",pid);
}