#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int account_balance = 0;
pthread_mutex_t lock;

void* increment(void *args) {
    int *n = args;
    pthread_mutex_lock(&lock);
    for(int j=0; j<*n; j++){
        for (int i = 0; i < 1000000; i++) {
            account_balance++;
        }
    }
    pthread_mutex_unlock(&lock);
}

int stringToInt(char *s){
    int a=0;
    for(int i=0; s[i]!='\0'; i++){
        a=a*10+(s[i]-'0');
    }
    return a;
}

int main(int argc, char* argv[]) {
    int threadNum = stringToInt(argv[1]);
    int N=2048;
    int n=N/threadNum;
    clock_t t1,t2;
    pthread_t th[threadNum];
    int i;
    for (i = 0; i < threadNum; i++) {
        if (pthread_create(th + i, NULL, &increment, &n) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        printf("Transaction %d has started\n", i);
    }
    t1 = clock();
    for (i = 0; i < threadNum; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Transaction %d has finished\n", i);
    }
    t2 = clock();
    pthread_mutex_destroy(&lock);

    printf("Account Balance is : %d\n", account_balance);
    printf("Time spent: %f\n", (double)(t2-t1)*1000/CLOCKS_PER_SEC);
    return 0;
}