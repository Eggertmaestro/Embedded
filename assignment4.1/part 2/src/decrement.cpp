#include "../include/decrement.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdint>

// External variables
extern pthread_mutex_t sharedVariableMutex;
extern int32_t gSharedVariable;

void *decrementTask(void *param) {
    while (1) {
        sleep(7);
        pthread_mutex_lock(&sharedVariableMutex);
        gSharedVariable--;
        printf("Decrement Task: shared var is %d\n", gSharedVariable);
        pthread_mutex_unlock(&sharedVariableMutex);
    }
}