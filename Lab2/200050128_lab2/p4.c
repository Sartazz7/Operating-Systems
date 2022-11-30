#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define N 50
#define FILE_SIZE 10000

int main(){
    char fileName1[N], fileName2[N];
    scanf("%s",fileName1);
    scanf("%s",fileName2);

    char content[FILE_SIZE];

    int fd1=open(fileName1,O_RDONLY);
    int fileSize=read(fd1,content,FILE_SIZE);
    close(fd1);

    int fd2=open("file2",O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd2,content,fileSize);
    close(fd2);
}