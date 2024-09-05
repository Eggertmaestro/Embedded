// ------------------------------------------------- Part 1 and 2 -------------------------------------------------

#include <Arduino.h>
#include "Encoder.h"

// Encoder encoder(0, 1); 

// void setup() {
//     Serial.begin(57600);  
// }

// void loop() {
//     encoder.update();  
//     int position = encoder.getPosition();

//     char str[10];
//     itoa(position, str, 10);  
//     Serial.println(str);  
// }

// ------------------------------------------------- Part 3 -------------------------------------------------

Encoder encoder(0, 1); 

void setup() {
    Serial.begin(57600);  
}

void loop() {
    encoder.update();  
    int position = encoder.getPosition();

    char str[10];
    itoa(position, str, 10);  
    Serial.println(str);  
 }