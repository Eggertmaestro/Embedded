#!/bin/bash
CHIP="gpiochip0"  # GPIO chip for Raspberry Pi
INPUT_PIN=17      # Encoder input pin
OUTPUT_PIN=27     # Output pin to toggle

# Export GPIO pins for use with gpiod
gpioset $CHIP $OUTPUT_PIN=0  # Initialize output pin to low state

# Main loop to read the encoder input pin and copy the state to the output pin
while true; do
    # Read the input pin state (encoder)
    STATE=$(gpioget $CHIP $INPUT_PIN)

    # Output the current state of the pin
    #echo "Encoder pin state: $STATE"

    # Set the output pin to match the input pin state
    gpioset $CHIP $OUTPUT_PIN=$STATE

    # Small delay to avoid CPU overload
    sleep 0.01  
done