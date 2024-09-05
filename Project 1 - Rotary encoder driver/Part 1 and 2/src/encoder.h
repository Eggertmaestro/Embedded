// ------------------------------------------------- part 1 and 2 -------------------------------------------------


#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <digital_in.h>

// class Encoder {
// private:
//     Digital_in pin1;
//     Digital_in pin2;
//     int num;
//     bool pin1_state;
//     bool pin2_state;
//     bool pin1_lastState;
//     bool pin2_lastState;

// public:
//     Encoder(int pin1_num, int pin2_num);
//     void update();
//     int getPosition() const;
// };

// #endif

// ------------------------------------------------- part 3 -------------------------------------------------

class Encoder {
private:
    Digital_in pin1;
    Digital_in pin2;
    int num;
    bool pin1_state;
    bool pin2_state;
    bool pin1_lastState;
    bool pin2_lastState;

public:
    Encoder(int pin1_num, int pin2_num);
    void update();
    int getPosition() const;
};

#endif