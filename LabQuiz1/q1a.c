#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 50

int main(){
	
	// Child process 1 for Alice
	int pid1=fork();
	if(pid1==0){
		char fileName[] = "secret1.txt";
		int fd=open(fileName, O_RDONLY);
		char secret1[N];
		int sz=read(fd,secret1,N);
		secret1[sz]='\0';
		close(fd);
		printf("%d: Secret code for Alice is %s\n", getpid(), secret1);
		exit(0);
	}
	
	// Waiting for Alice's process to end
	int status1;
	waitpid(pid1,&status1,0);
	
	// Child process 2 for Bob
	int pid2=fork();
	if(pid2==0){
		char fileName[] = "secret2.txt";
		int fd=open(fileName, O_RDONLY);
		char secret2[N];
		int sz=read(fd,secret2,N);
		secret2[sz]='\0';
		close(fd);
		printf("%d: Secret code for Bob is %s\n", getpid(), secret2);
		exit(0);
	}
	
	// Waiting for Bob's process to end
	int status2;
	waitpid(pid2,&status2,0);
	
	// Reaping the child processes
	kill(pid1,SIGINT);
	kill(pid2,SIGINT);
	
	return 0;
}
