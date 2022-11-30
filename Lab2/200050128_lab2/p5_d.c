#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    int pipeAB[2];
    int pipeBC[2];
    pipe(pipeAB);
    pipe(pipeBC);
    if(fork() == 0){
        printf("Process A : Read File Descriptor Value: %d\n", pipeAB[0]);
        printf("Process A : Write File Descriptor Value: %d\n", pipeAB[1]);
        sleep(1);
        printf("Process A : Input value of x : ");
        char x[50];
        scanf("%s", x);
        write(pipeAB[1], x, sizeof(x));
    } else if(fork() == 0) {
        printf("Process B : Read File Descriptor Value: %d\n", pipeAB[0]);
        printf("Process B : Write File Descriptor Value: %d\n", pipeAB[1]);
        printf("Process B : Read File Descriptor Value: %d\n", pipeBC[0]);
        printf("Process B : Write File Descriptor Value: %d\n", pipeBC[1]);
        sleep(1);
        char x[50];
        read(pipeAB[0], x, sizeof(x));
        printf("Process B : Input value of y : ");
        char y[50];
        scanf("%s", y);
        int add = atoi(x) + atoi(y);
        char sum[50];
        sprintf(sum, "%d", add);
        write(pipeBC[1], sum, sizeof(sum));
    } else if(fork() == 0) {
        printf("Process C : Read File Descriptor Value: %d\n", pipeBC[0]);
        printf("Process C : Write File Descriptor Value: %d\n", pipeBC[1]);
        sleep(1);
        char sum[50];
        read(pipeBC[0], sum, sizeof(sum));
        printf("Process C : Result after addition : %s\n", sum);
    } else {
        while(wait(NULL) > 0);
    }
    return 0;
}