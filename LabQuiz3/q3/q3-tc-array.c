#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int arr1[12] = {7,14,2,3,5,6,9,10,12,1, 4, 8};
int arr2[12] = {1,2,3,4,5,6,7,8,9,10, 11, 12};
int arr_size = 12;
int global_sum = 159;
int thread_sums[2][2] = {{0, 0}, {0, 0}};

void array_sum(int *arg1, int *arg2) {
    int sum = 0;
    if(*arg1 == 0 && *arg2 == 2) {
        for(int i=0;i<arr_size/4;i++) {
            sum += arr1[i];
        }
        for (int i=0;i<arr_size/4;i++) {
            sum += arr2[i];
        }
    }
    else if(*arg1 == 0 && *arg2 == 3) {
        for(int i=arr_size/4;i<arr_size/2;i++) {
            sum += arr2[i];
        }
        for (int i=arr_size/4;i<arr_size/2;i++) {
            sum += arr1[i];
        }
    }
    else if(*arg1 == 1 && *arg2 == 2){
        for(int i=arr_size/2;i<3*arr_size/4;i++) {
            sum += arr1[i];
        }
        for (int i=arr_size/2;i<3*arr_size/4;i++) {
            sum += arr2[i];
        }
    }
    else if(*arg1 == 1 && *arg2 == 3){
        for(int i=3*arr_size/4;i<arr_size;i++) {
            sum += arr2[i];
        }
        for (int i=3*arr_size/4;i<arr_size;i++) {
            sum += arr1[i];
        }
    }   
    else{
        printf(1, "Invalid arguments\n");
        exit();
    }
    
    
    thread_sums[*arg1][*arg2 - 2] = sum;
    printf(1, "Argument 1: %d, Argument 2: %d Sum Value: %d\n", *arg1, *arg2, sum);
    exit();
}

int main(int argc, char *argv[]) {

    printf(1, "Creating threads to sum up two arrays\n");

    for(int i=0;i<2;i++) {
        for(int j=2;j<4;j++) {
            create_thread(array_sum, &i, &j);
            sleep(100);
        }
    }

    for(int i=0;i<2;i++) {
        for(int j=2;j<4;j++) {
            join();
        }
    }

    printf(1, "All threads joined\n");
    int thread_sum = 0;
    for(int i=0;i<2;i++) {
        for(int j=2;j<4;j++) {
            thread_sum += thread_sums[i][j-2];
        }
    }

    if (thread_sum == global_sum) {
    	printf(1, "Sum of thread calls is equal to that of both array sums, value: %d\n", global_sum);
    }
    else {
    	printf(1, "Sum of thread calls is not equal to that of both array sums, thread sums: %d and array sum: %d\n", thread_sum, global_sum);
    }
    
    
    exit();
}
