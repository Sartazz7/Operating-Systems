#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
  
int main(int argc, char *argv[]){

	// Child process to execute the executable from argv[1]
	int child=fork();
	if(child==0){
		char newargs[1] = "";
		printf("pid %d: I am child\n",getpid());
		execl(argv[1],newargs,(char*)NULL);
		exit(0);
	}
	
	// Another child process to check for timeout
	int pid1=fork();
	if(pid1==0){
		sleep(5);
		kill(child,SIGINT);
		printf("Terminating process %d\n",child);
		exit(0);
	}
	
	// Waits for child process to end either by itself or by timeout
	int status;
	waitpid(child,&status,0);
	
	// killing the child processes
	kill(child,SIGINT);
	sleep(1); // To ensure timeout message is printed before killing the 2nd child
	kill(pid1,SIGINT);
	return 0;
}
