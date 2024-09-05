// ------------------------------------------------- digital_in.cpp -------------------------------------------------

#include <avr/io.h>        // Register definitions
#include "digital_in.h"    // Digital_out class

// Constructor
Digital_in::Digital_in(int pin) {
    // Create a bitmask for the given pin
    pinMask = (1 << pin);
}

// Initialize the pin as an input
void Digital_in::init() {
    DDRB &= ~pinMask;  // Clear the corresponding pin in DDRB, making it an input
}

// Check if the input is high
bool Digital_in::is_hi() {
    return PINB & pinMask;  // Return true if the pin is high
}

// Check if the input is low
bool Digital_in::is_lo() {
    return !(PINB & pinMask);  // Return true if the pin is low
}