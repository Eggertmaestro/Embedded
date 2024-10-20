#ifndef PI_CONTROL_H
#define PI_CONTROL_H

class PI_Control {
private:
    double Kp;
    double Ti;
    double T;
    double integral;
    double saturation_limit;

public:
    PI_Control(double Kp, double Ti, double T, double saturation_limit);
    double update(double setpoint, double actual, double dt);
    void reset_integral();
};

#endif
