// include guard
#ifndef __PID_Controller_H__
#define __PID_Controller_H__

// dependencies
#include <stdlb.h>
#include <iostream>

/**
    PIDController object abstracts the math required to compute the duty cycle a
        motor should be spinning at inorder to maintain a desired equalibrium
 */
class PIDController {


private:
    /**
        `Porportional` constant for the PID Loop
     */
    double k_P          const;

    /**
        `Integral` constant for the PID Loop
     */
    double k_I          const;

    /**
        `Derivative` constant for the PID Loop
     */
    double k_D          const;

    /**
        MAX RPM the motor this PIDController is controlling can spin at
     */
    double k_MAX_RPM    const;

    /**
        MAX TORQUE the motor this PIDController is controlling can apply
     */
    double k_MAX_TORQUE const;

    /**
        Integral of the error (target{0} - current_position)
     */
    double accumulated_error;


public:
    /**
        Creates a PIDController object with the specified PID constants and max
            rpm/torque
     */
    PIDController(double P, double I, double D, double MAX_RPM,
        double MAX_TORQUE);

    /**
        Destroys a PIDController object
     */
    ~PIDController();

    /**
        returns: the desired dutyCycle of the PWM Signal going to the motor
            ranging from -1 to 1
        note: a negtive sign means the motor should spin in the oppiste
            direction
     */
    double dutyCycle(double bike_theta, double bike_theta_dot,
        double motor_theta_dot);
}

#endif // __PID_Controller_H__