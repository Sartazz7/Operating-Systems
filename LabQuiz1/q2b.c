#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

int pid;

// SIGINT handler used by parent to kill the child process
void sigintHandler(int signum){
	kill(pid,SIGINT);
	printf("\n");
}

// SIGQUIT handler for parent to exit
void sigquitHandler(int signum){
	printf("\nShell is exiting\n");
	exit(0);
}

int main(){
	char command[256];
	char newargs[1] = "";
	
	signal(SIGINT,sigintHandler);
	signal(SIGQUIT,sigquitHandler);
	
	while (1){
		char prompt[] = ">>> ";
		printf("%s",prompt);
		scanf("%s",command);
		pid = fork();
		if(pid==0){
			execl(command,newargs,(char*)NULL);
		    	return 1;
		}
		else{
		    	wait(NULL);
		}
	}
    
    return 0;
}
