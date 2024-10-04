#ifndef FIFO_H
#define FIFO_H

#include <cstddef> // for size_t
#include <array>
#include <mutex>

const size_t FIFO_SIZE = 10; // Define the maximum size of the FIFO

class Fifo {
public:
    Fifo(); // Constructor

    void put(int item); // Add an item to the FIFO
    int get(); // Retrieve an item from the FIFO
    bool is_empty(); // Check if the FIFO is empty
    bool is_full(); // Check if the FIFO is full
    void reset(); // Reset the FIFO

private:
    std::array<int, FIFO_SIZE> buffer; // Circular buffer
    size_t front; // Index of the front item
    size_t back; // Index of the back item
    size_t count; // Current count of items in the FIFO
    std::mutex mtx; // Mutex for thread-safe access
};

#endif // FIFO_H