#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
  
int main(){
    int pid;
    int n;
    scanf("%d",&n);
    pid = fork();
    if (pid == 0){
        for(int i=0; i<n; i++){
            printf("C %d %d\n", getpid(), i+1);
        }
    }
    else {
        for(int i=0; i<n; i++){
            printf("P %d %d\n", getpid(), i+n+1);
        }
    }
    return 0;
}