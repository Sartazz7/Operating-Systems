#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigintHandler(int sig_num){
    printf("SIGINT\n");
}

void sigtermHandler(int sig_num){
    printf("SIGTERM\n");
}

int main(){
    printf("Process Id is: %d\n",getpid());
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    while(1){
        printf("Waiting...\n");
        sleep(3);
    }
    return 0;
}