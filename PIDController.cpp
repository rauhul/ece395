#include "PIDController.h"

/**
    Creates a PIDController object with the specified PID constants and max
        rpm/torque
 */
PIDController::PIDController(double P, double I, double D, double MAX_RPM,
        double MAX_TORQUE) {
    k_P = P;
    k_I = I;
    k_D = D;
    k_MAX_RPM    = MAX_RPM;
    k_MAX_TORQUE = k_MAX_TORQUE;
}

/**
    Destroys a PIDController object
 */
PIDController::~PIDController() {
    // nothing to do here
}

/**
    returns: the desired dutyCycle of the PWM Signal going to the motor
        ranging from -1 to 1
    note: a negtive sign means the motor should spin in the oppiste
        direction
 */
double PIDController::dutyCycle(double bike_theta, double bike_theta_dot,
    double motor_theta_dot) {
    //time stamp (for error) (get C time, store last Ctime called)

    double current_error = 0 - bike_theta;
    accumulated_error += current_error;
    double derivative = bike_theta_dot;

    // compute response torque based on system state and PID constants
    double torque = (k_P * current_error) + (k_I * accumulated_error) + (k_D * derivative);

    /**
        assumption: the maximum torque the motor can produce at a given angular
            velocity is the same in both rotational directions
     */

    // compute the maximum torque the motor can produce at it's current speed
    double torque_limit = k_MAX_TORQUE * (1 - (abs(motor_theta_dot)/k_MAX_RPM));

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
