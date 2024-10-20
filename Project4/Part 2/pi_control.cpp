#include "pi_control.h"
#include <stdio.h>

PI_Control::PI_Control(double Kp, double Ti, double T, double saturation_limit)
    : Kp(Kp), Ti(Ti), T(T), saturation_limit(saturation_limit), integral(0) {}

double PI_Control::update(double setpoint, double actual, double dt) {
    double error = setpoint - actual;
    double proportional = Kp * error;

    // Integrate using actual elapsed time
    integral += error * dt;

    // Compute the integral term
    double integral_term = integral / Ti;

    // Calculate the control output
    double output = proportional + integral_term;

    // Clamp the output to avoid overshoot and windup
    if (output > saturation_limit) {
        output = saturation_limit;
    } else if (output < 0) {
        output = 0;
    }

    // Enhanced debug output to track changes
    printf("Actual: %.2f,Error: %.2f\n", actual, error);

    return output; // Return the control output
}

void PI_Control::reset_integral() {
    integral = 0;  // Reset the integral term
}
