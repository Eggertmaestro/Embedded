#include <Arduino.h>
#include "Encoder.h"
#include "digital_out.h"
#include <avr/interrupt.h>

//Initialize encoder and digital output objects
Encoder encoder(0, 2); 
Digital_out led(5);

//Arduino setup function
void setup() {
    Serial.begin(57600);
    while (!Serial)
    {
      ; // wait for serial port to connect. Needed for Native USB only
    }  
}

int main() {
    led.init();
    while(1){
        float deg = encoder.getPosition() /700.0 * 360; // Calculate the motor's angular position in degrees

        // Print encoder counter and calculated degrees to the serial monitor
        Serial.print(encoder.getPosition());
        Serial.print("  Deg:  ");
        Serial.println(deg);
        _delay_us(1000);
    }
}

//Interrupt Service Rountine (ISR) for external interupt INT0 
ISR(INT0_vect)
{
    if (encoder.pin1.is_hi()) // Check if encoder pin1 is high
    {
        encoder.count++; // Increment the encoder count (moving in one direction)
        led.set_hi(); // Turn on LED to signal change
        _delay_us(1); // Small delay to debounce
        led.set_lo(); // Turn of LED
    }
    
    else
    {
        encoder.count--; // Decrement the encoder count (moving in the opposite direction)
        led.set_hi(); 
        _delay_us(1);
        led.set_lo();
    }

}

