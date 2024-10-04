#include "../include/fifo.h"

Fifo::Fifo() : front(0), back(0), count(0) {
    buffer.fill(0); // Initialize the buffer to zero
}

void Fifo::put(int item) {
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex
    if (is_full()) {
        // When full, overwrite the oldest item.
        front = (front + 1) % FIFO_SIZE; // Move front ahead to allow overwriting
    } else {
        count++;
    }
    buffer[back] = item; // Insert item at the back
    back = (back + 1) % FIFO_SIZE; // Move back pointer
}

int Fifo::get() {
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex
    if (is_empty()) {
        return -1; // Indicate empty FIFO
    }
    int item = buffer[front]; // Retrieve item at front
    front = (front + 1) % FIFO_SIZE; // Move front pointer forward
    count--; // Decrease count
    return item; // Return retrieved item
}

bool Fifo::is_empty() {
    return count == 0; // Return true if FIFO is empty
}

bool Fifo::is_full() {
    return count == FIFO_SIZE; // Return true if FIFO is full
}

void Fifo::reset() {
    front = 0; // Reset front pointer
    back = 0;  // Reset back pointer
    count = 0; // Reset count
}