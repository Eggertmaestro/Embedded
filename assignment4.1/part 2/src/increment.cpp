#include "../include/increment.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdint> 

// External variables
extern pthread_mutex_t sharedVariableMutex;
extern int32_t gSharedVariable;

void *incrementTask(void *param) {
    while (1) {
        sleep(3);
        pthread_mutex_lock(&sharedVariableMutex);
        gSharedVariable++;
        printf("Increment Task: shared var is %d\n", gSharedVariable);
        pthread_mutex_unlock(&sharedVariableMutex);
    }
}