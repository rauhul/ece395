#include "PIDController.h"

/**
    Creates a PIDController object with the specified PID constants and max
        rpm/torque

    - parameters:
        - P: `Porportional` constant for the PID Loop
        - I: `Integral` constant for the PID Loop
        - D: `Derivative` constant for the PID Loop
        - MAX_RPM: The maximum RPM the motor this PIDController is
            controlling can spin at
        - MAX_TORQUE: The maximum torque the motor this PIDController is
            controlling can apply
        - TARGET: The equilibium point for the PIDController to maintain
 */
PIDController::PIDController(double P, double I, double D, double MAX_RPM,
        double MAX_TORQUE, double TARGET) {
    k_P = P;
    k_I = I;
    k_D = D;
    k_MAX_RPM    = MAX_RPM;
    k_MAX_TORQUE = k_MAX_TORQUE;
    k_TARGET     = TARGET;
}

/**
    Destroys a PIDController object
 */
PIDController::~PIDController() {
    // nothing to do here
}

void PIDController::start(double current_position) {
    previous_position = current_position;
    previous_time = time(NULL);
}

/**
    returns: the desired dutyCycle of the PWM Signal going to the motor
        ranging from -1 to 1
    note: a negtive sign means the motor should spin in the oppiste
        direction
 */
double PIDController::dutyCycle(double current_position, double motor_omega) {
    time_t current_time = time_t(NULL);
    time_t time_diff = current_time - previous_time;
    previous_time = current_time;

    double current_error = k_TARGET - bike_theta;
    accumulated_error += current_error * ((double) time_diff);
    double derivative = (current_position - prev_position) / ((double) time_diff);

    // compute response torque based on system state and PID constants
    double torque = (k_P * current_error) + (k_I * accumulated_error) +
        (k_D * derivative);

    /**
        assumption: the maximum torque the motor can produce at a given angular
            velocity is the same in both rotational directions
     */

    // compute the maximum torque the motor can produce at the current angular
    //      velocity
    double torque_limit = k_MAX_TORQUE * (1 - (abs(motor_omega)/k_MAX_RPM));

    // convert torque to PWM duty cycle
    double duty_cycle = torque/torque_limit;

    // clamp duty cycle range to [-1: 1]
    if (duty_cycle > 1) {
        duty_cycle = 1;
    } else if (duty_cycle < -1) {
        duty_cycle = -1;
    }

    return duty_cycle
}
