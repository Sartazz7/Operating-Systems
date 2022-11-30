#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int VAR = 0;

void increment(int *arg1, int *arg2) {
    VAR++;
    printf(1, "Sum of args: %d, VAR: %d\n", (*arg1 + *arg2), VAR);
    exit();
}

int main(int argc, char *argv[]) {

    printf(1, "Calling Process Print VAR value: %d\n", VAR);
    int N = 5;
    int zero = 0;

    for(int i=0;i<N;i++) {
        create_thread(increment, &i, &zero);
        sleep(100);
    }

    for(int i=0;i<N;i++) {
        join();
    }

    printf(1, "All threads joined, VAR value: %d\n", VAR);
    exit();
}