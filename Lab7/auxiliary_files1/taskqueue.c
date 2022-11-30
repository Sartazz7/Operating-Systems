#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Global variables
long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;

char *processType;
int *processTime;
int start=0, end=0;

pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_t *th;

typedef struct{
    int num;
    int tid;
} Data;

void *getTask(void *args);

void processtask(long number, int tid)
{
    // simulate burst time
    sleep(number);

    pthread_mutex_lock(&lock1);
    // update global variables
    sum += number;
    if (number % 2 == 1)
    {
        odd++;
    }
    else
    {
        even++;
    }
    if (number < min)
    {
        min = number;
    }
    if (number > max)
    {
        max = number;
    }
    pthread_mutex_unlock(&lock1);

    pthread_create(th + tid, NULL, &getTask, &tid);
    pthread_join(th[tid], NULL);
}

void waittask(long number, int tid){
    sleep(number);
    pthread_create(th + tid, NULL, &getTask, &tid);
    pthread_join(th[tid], NULL);
}

void *getTask(void *args){
    int allocated;
    int *tid = args;
    pthread_mutex_lock(&lock2);
    while(start==end){
        if(done){
            pthread_mutex_unlock(&lock2);
            return NULL;
        }
    }
    allocated = start;
    start++;
    pthread_mutex_unlock(&lock2);

    if(processType[allocated]=='p'){
        processtask(processTime[allocated],*tid);
    }
    else {
        waittask(processTime[allocated],*tid);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: sum <infile>\n");
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    // Read from file
    FILE *fin = fopen(fn, "r");
    long t;
    int workers=atoi(argv[2]);
    th = (pthread_t*)malloc(workers*sizeof(pthread_t));
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    printf("The number of workers are : %d \n", workers);

    for(int tid=0; tid<workers; tid++){
        pthread_create(th + tid, NULL, &getTask, &tid);
    }

    processType = (char *)malloc(t*sizeof(char));
    processTime = (int *)malloc(t*sizeof(int));
    char type;
    long num;
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {
        if (type == 'p')
        {
            processType[end] = 'p';
            processTime[end] = num;
            end++;
        }
        else if (type == 'w')
        {
            processType[end] = 'w';
            processTime[end] = num;
            end++;
        }
        else
        {
            printf("ERROR: Type Unrecognizable: '%c'\n", type);
            exit(EXIT_FAILURE);
        }
    }
    
    for(int tid=0; tid<workers; tid++){
        pthread_join(th[tid], NULL);
    }
    fclose(fin);
    // Print global variables
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    return (EXIT_SUCCESS);
}
