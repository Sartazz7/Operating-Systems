/* tour.c
* To learn the basic signal handling in c
* Can send the interrupt signal via Ctrl-c in a terminal.
*
* Complete TODO items for this question
*/

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_FRIEND 5 // Number of friends visiting the Dinosaurs Park

int friend_count = 0; // Current count of friends completed the tour

// TODO: Add a signal handling function which handle SIGINT and SIGCHLD signals

void sigintHandler(int signum, siginfo_t *info, void *ptr){
    printf("\n\nYou have interrupted the tour.\n");
    if(friend_count==NUM_FRIEND){
        printf("All 5 friends have completed the tours.\n");
        printf("Thank you for visiting the Dinosaurs park\n");
        exit(0);
    }
    else {
        printf("Oh Sorry! Only %d out of the %d friends have completed the tour.\n", friend_count, NUM_FRIEND);
    }
}

void sigchildHandler(int signum, siginfo_t *info, void *ptr){
    friend_count++;
}

// END TODO
int main(int argc, char *argv[]){

    printf("Welcome to the Dinosaurs Park.\n");

    /* 
    * TODO: add struct/calls to sigaction() to handle SIGINT and SIGCHLD. 
    * Use sigaction() and associated structs.
    * Ensure that the flag SA_RESTART is set as well to ensure system calls are automatically restarted.
    */
    
    // signal(SIGINT,sigintHandler);
    // signal(SIGCHLD,sigchildHandler);

    struct sigaction sigint,sigchild;

    sigint.sa_sigaction = sigintHandler;
    sigint.sa_flags = SA_RESTART;
    sigaction(SIGINT,&sigint,NULL);

    sigchild.sa_sigaction = sigchildHandler;
    sigchild.sa_flags= SA_RESTART;
    sigaction(SIGCHLD,&sigchild,NULL);

    // END TODO
    printf("The Process ID of Dinosaurs Park: %d \n", getpid());

    for (size_t friend_num = 1; friend_num <= NUM_FRIEND; friend_num++){
        int pid=fork();
        if (pid == 0){
            //TODO Note that, you must carefully place the various children in different process groups 
            // that is different from the default process group of its parent
            //say, using the setpgid system call.

            signal(SIGINT,SIG_IGN);
            setpgid(getpid(),getpid());
            // printf("getpid(): %d, getpgid(getpid()): %d\n",getpid(), getpgid(getpid()));

            sleep(5 * friend_num);             // sleep emulates "touring" time
            printf("Friend #%zu with process ID - %d has completed the tour.\n", friend_num, getpid());
            exit(0);
            //END TODO
        }
    }
    for(;;)pause(); // wait for all friends to complete tour
    return 0;
}
