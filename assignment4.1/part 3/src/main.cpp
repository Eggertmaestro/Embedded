#include <iostream>
#include <thread>
#include "../include/increment.h"
#include "../include/decrement.h"
#include "../include/fifo.h"

// Global declarations
Fifo fifo; // FIFO buffer
int32_t gCounter = 0; // Shared counter variable

int main() {
    // Create threads for incrementing and decrementing tasks
    std::thread incrementThread(incrementTask, nullptr);  // Start increment thread
    std::thread decrementThread(decrementTask, nullptr);  // Start decrement thread

    incrementThread.join();  // Join increment thread
    decrementThread.join();  // Join decrement thread

    return 0;
}