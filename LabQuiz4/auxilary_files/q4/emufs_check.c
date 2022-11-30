#include "emufs.h"


void check_disk(char* disk_name){
    int mnt = opendevice(disk_name);
    
    if(mnt==-1){
        printf("error!\n");
        return;
    }

    filesystem_check(mnt);
}

int main(int argv, char *argc[]){
    check_disk("disk1");
    check_disk("disk2");
    check_disk("disk3");
    check_disk("disk4");
    check_disk("disk5");
    check_disk("disk6");
    return 0;
}