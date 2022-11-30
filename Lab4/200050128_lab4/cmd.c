#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    int pid = fork();
    if(pid==0){
        exec(argv[1],&argv[1]);
        exit();
    }
    wait();
    exit();
}
