#include "../include/increment.h"
#include "../include/fifo.h"
#include <iostream>
#include <thread>
#include <chrono>

extern Fifo fifo; // FIFO instance
extern int32_t gCounter; // Shared counter variable

void* incrementTask(void* param) {
    for (int i = 0; i < 10; ++i) { // Write 10 values to FIFO
        fifo.put(gCounter); // Insert counter value into FIFO
        std::cout << "Written to FIFO: " << gCounter << std::endl;
        gCounter++; // Increment counter
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return nullptr;
}