#include "../include/increment.h"
#include "../include/decrement.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdint> 

// Global variable declarations
pthread_mutex_t sharedVariableMutex;
int32_t gSharedVariable = 0;

int main(void) {
    pthread_t incrementTaskObj, decrementTaskObj;

    // Initialize the mutex
    pthread_mutex_init(&sharedVariableMutex, NULL);

    // Create increment and decrement tasks
    pthread_create(&incrementTaskObj, NULL, incrementTask, NULL);
    pthread_create(&decrementTaskObj, NULL, decrementTask, NULL);

    // Wait for tasks to complete
    pthread_join(incrementTaskObj, NULL);
    pthread_join(decrementTaskObj, NULL);

    pthread_mutex_destroy(&sharedVariableMutex);

    return 0;
}