#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int N, K;
int C;
sem_t barrier1, barrier2;


// Thread function
void* worker(void* args){
    // Handle the function arguments
    int thread_num = *((int*)args);
    for(int ii=0; ii<K; ii++){
        // Wait for all workers to finish previous loop
        sem_wait(&barrier1);

        C += 1;  // Updating a shared variable 
        sem_post(&barrier1);

        // Wait for all workers to finish incrementing
        sem_wait(&barrier2);

        printf("thread_num: %d, C = %d\n", thread_num, C);
        sem_post(&barrier2);
    }
        
    return NULL;
}

int main(int argc, char *argv[]){
    printf("N: ");
    scanf("%d", &N);

    printf("K: ");
    scanf("%d", &K);
    
    C = 0;

    // Initialize the semaphores you create
    sem_init(&barrier1,N,0);
    sem_init(&barrier2,N,0);


    // Create N worker threads with worker() thread function
    pthread_t threads[N];
    int id[N];

    // for(int i=0; i<N; i++){
    //     id[i] = i;
    //     pthread_create(threads+i,NULL,&worker,&id[i]);
    // }

    // for(int i=0; i<K; i++){
    //     for(int j=0; j<N; j++){
    //         sem_post(&barrier1);
    //     }
    //     sleep(1);
    //     for(int j=0; j<N; j++){
    //         sem_post(&barrier2);
    //     }
    // }
    sem_post(&barrier1);
    sem_post(&barrier2);

    // Join the threads
    for(int i=0; i<N; i++){
        pthread_join(threads[i],NULL);
    }

    return 0;
}