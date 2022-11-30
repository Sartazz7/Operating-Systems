#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Global variables
long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;

// task variables
char *taskType;
long *taskTime;
int start = 0;
int end = 0;

FILE *fin;
long t;

// thread variables
pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_t mainTh;
pthread_t *th;

void processtask(long number);
void waittask(long number);

void processtask(long number){
    sleep(number);
    pthread_mutex_lock(&lock1);
    sum += number;
    if (number % 2 == 1)odd++;
    else even++;
    if (number < min)min=number;
    if (number > max)max=number;
    pthread_mutex_unlock(&lock1);
}

void waittask(long number){
    sleep(number);
}

void *mainThread(){
    char type;
    long num;
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2){
        taskType[end] = type;
        taskTime[end] = num;
        end++;
    }
    fclose(fin);
}

void *workerThread(){
    while(true){
        int allocated;
        pthread_mutex_lock(&lock2);
        while(start >= end){
            if(start >= t)break;
        }
        allocated = start;
        start++;
        pthread_mutex_unlock(&lock2);
        if(allocated >= t)break;
        if(taskType[allocated] == 'p')processtask(taskTime[allocated]);
        else waittask(taskTime[allocated]);
    }
}

int main(int argc, char *argv[]){
    char *fn = argv[1];
    fin = fopen(fn, "r");
    fscanf(fin, "%ld\n", &t);
    taskType = (char*)malloc(t*sizeof(char));
    taskTime = (long*)malloc(t*sizeof(long));

    int threadNum = atoi(argv[2]);
    th = (pthread_t*)malloc(threadNum*sizeof(pthread_t));
    clock_t t1,t2;

    t1 = clock();
    pthread_create(&mainTh,NULL,&mainThread,NULL);
    for(int i=0; i<threadNum; i++){
        pthread_create(th+i,NULL,&workerThread,NULL);
    }
    pthread_join(mainTh,NULL);
    for(int i=0; i<threadNum; i++){
        pthread_join(th[i],NULL);
    }
    t2 = clock();

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);
    printf("Time spent: %f\n",(double)(t2-t1)/CLOCKS_PER_SEC);

    return (EXIT_SUCCESS);
}
