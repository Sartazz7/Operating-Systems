#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 8

int stringToInteger(char s[]){
    int a=0;
    for(int i=0; s[i]!='\0'; i++){
        a=a*10+(s[i]-'0');
    }
    return a;
}

char *integerToString(int a){
    char *s;
    s=(char *)malloc(N*sizeof(char));
    int n=0;
    while(a>0){
        s[n]=(char)('0'+a%10);
        a/=10;
        n++;
    }
    char *t;
    t=(char *)malloc(N*sizeof(char));
    for(int i=n-1; i>=0; i--){
        t[n-i-1]=s[i];
    }
    t[n]='\0';
    return t;
}

int main(){
    int pipefdsAB[2],pipefdsBC[2];
    pipe(pipefdsAB);
    pipe(pipefdsBC);
    if(fork()==0){
        printf("Process A: Input value of x: ");
        char x[N];
        scanf("%s",x);
        write(pipefdsAB[1],x,N);
    }
    else if(fork()==0){
        char x[N];
        read(pipefdsAB[0],x,N);
        printf("Process B: Input value of y: ");
        char y[N];
        scanf("%s",y);
        int add=atoi(x)+atoi(y);
        char sum[N];
        sprintf(sum, "%d", add);
        write(pipefdsBC[1],sum,N);
    }
    else if(fork()==0){
        char sum[N];
        read(pipefdsBC[0],sum,N);
        printf("Process C: Result after addition: %s\n",sum);
    }
    else {
        while(wait(NULL)>0);
    } 
}