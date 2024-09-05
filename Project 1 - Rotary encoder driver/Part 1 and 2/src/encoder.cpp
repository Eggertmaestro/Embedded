// ------------------------------------------------- Part 1 and 2 -------------------------------------------------

#include "Encoder.h"

// Encoder::Encoder(int pin1_num, int pin2_num) 
//     : pin1(pin1_num), pin2(pin2_num), num(0),
//       pin1_state(false), pin2_state(false), 
//       pin1_lastState(false), pin2_lastState(false) {}

// void Encoder::update() {
//     pin1_state = pin1.is_hi();
//     pin2_state = pin2.is_hi();

//     if ((pin1_state && !pin1_lastState) || (!pin1_state && pin1_lastState)) {
//         if (pin1_state == pin2_state) {
//             num++;
//         } else {
//             num--;
//         }
//         digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED to indicate state change
//         delay(1);  // Short delay for LED indication
//         digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
//     }

//     pin1_lastState = pin1_state;
//     pin2_lastState = pin2_state;
// }

// int Encoder::getPosition() const {
//     return num;
// }

// ------------------------------------------------- Part 3 -------------------------------------------------

Encoder::Encoder(int pin1_num, int pin2_num) 
    : pin1(pin1_num), pin2(pin2_num), num(0),
      pin1_state(false), pin2_state(false), 
      pin1_lastState(false), pin2_lastState(false) {}

void Encoder::update() {
    pin1_state = pin1.is_hi();
    pin2_state = pin2.is_hi();

    //if ((pin1_state && !pin1_lastState) || (!pin1_state && pin1_lastState)) {
    if ((!pin1_state && pin1_lastState)) {
        if (pin1_state == pin2_state) {
            num++;
            digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED to indicate state change
            digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
        } else {
            num--;
            digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED to indicate state change
            digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
        }
        
    }

    pin1_lastState = pin1_state;
    pin2_lastState = pin2_state;
}

int Encoder::getPosition() const {
    return num;
}