#include "../include/decrement.h"
#include "../include/fifo.h"
#include <iostream>
#include <thread>
#include <chrono>

extern Fifo fifo; // FIFO instance

void* decrementTask(void* param) {
    for (int i = 0; i < 10; ++i) { // Attempt to read 10 values
        int value = fifo.get(); // Retrieve value from FIFO
        if (value != -1) {
            std::cout << "Read from FIFO: " << value << std::endl; // Print the value
        } else {
            std::cout << "FIFO is empty, cannot read!" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(7)); // Simulate work
    }
    return nullptr;
}