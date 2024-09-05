// ------------------------------------------------- digital_in.h -------------------------------------------------

#include <stdint.h>  // standard integer types for uint8_t 
class Digital_in {
public:
    // Constructor: takes a pin number as an integer argument, where the pin number refers to the pin number within a port.
    Digital_in(int pin);

    // Initialize the pin as an input
    void init();

    // Check if the input is in high state
    bool is_hi();

    // Check if the input is in low state
    bool is_lo();

private:
    uint8_t pinMask; // Used in the register operations for the pin
};