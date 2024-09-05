#include "Encoder.h"
#include <avr/io.h>

Encoder::Encoder(int pin1_num, int pin2_num) : pin1(pin1_num, 'B'), pin2(pin2_num, 'D'){
         EICRA |= (1 << ISC01); //Sense interrupt on pin0 change rising edge
         EIMSK |= (1 << INT0);  //Enable interrupt on INT0
        
        sei(); // enable interrupts
    }

// Get the current position of the encoder
int Encoder::getPosition(){
    return count;
}
