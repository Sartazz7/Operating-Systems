#include "types.h"
#include "stat.h"
#include "user.h"

#define N 512

void printHeading(char *fileName){
    char heading[N];
    int i=0; 
    for(i=0; i<5; i++){
        heading[i]='-';
    }
    for(i=5; fileName[i-5]!='\0'; i++){
        heading[i]=fileName[i-5];
    }
    for(int j=0; j<5; j++){
        heading[i+j]='-';
    }
    heading[i+5]='\n';
    heading[i+6]='\0';
    write(1,heading,i+6);
}

char buf[1];

void head(int fd, int lines){
    int n;
    
    while((n = read(fd, buf, 1)) > 0) {
        if (write(1, buf, 1) != 1) {
            printf(1, "cat: write error\n");
            exit();
        }
        if(buf[0]=='\n')lines--;
        if(lines==0)break;
    }
    if(n < 0){
        printf(1, "cat: read error\n");
        exit();
    }
}

int stringToInteger(char *s){
    int a=0;
    for(int i=0; s[i]!='\0'; i++){
        a=a*10+(s[i]-'0');
    }
    return a;
}

int main(int argc, char *argv[]){
    int fd, i;

    if(argc <= 2){
        head(0,1);
        exit();
    }
    int lines = stringToInteger(argv[1]); 
    for(i = 2; i < argc; i++){
        if((fd = open(argv[i], 0)) < 0){
            printf(1, "cat: cannot open %s\n", argv[i]);
            exit();
        }
        printHeading(argv[i]);
        head(fd,lines);
        close(fd);
    }
    exit();
}
