#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void printSchedulerStats(int pid, int arrival_time, int first_scheduled_time, int exit_time) {
    printf(1, "pid: %d, arrival time: %d, first scheduled time: %d, exit time: %d\n", pid, arrival_time, first_scheduled_time, exit_time);
}

int main(int argc, char *argv[]){
    printf(1, "Parent process started\n");

    int NCHILD = 2;
    if(argc > 1){
        NCHILD = atoi(argv[1]);
    }

    printf(1, "Forking child processes\n");
    for(int i=0; i<NCHILD; i++){
        int pid = fork();
        if(pid == 0){
            volatile double a, b;
            a = 3.14;
            b = 36.29;
            volatile double x = 0, z;
            for (z = 0; z < 90000.0; z += 0.1)
            {
                x = x + a * b; 
            }
            exit();
        }
    }

    for(int i=0; i<NCHILD; i++){
        int arrival_time, first_scheduled_time, exit_time;
        int pid = wait2(&arrival_time, &first_scheduled_time, &exit_time);
        printSchedulerStats(pid, arrival_time, first_scheduled_time, exit_time);
    }

    printf(1, "Parent process has reaped all the child processes\n");
    exit();
}
