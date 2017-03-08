// include guard
#ifndef __PID_Controller_H__
#define __PID_Controller_H__

// dependencies
#include <stdlib.h>
#include <iostream>
#include <time.h>

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
        the equilibium point for the PIDController to maintain
    */
    double k_TARGET const;

    /**
        Integral of the error (k_TARGET - current_position)
     */
    double accumulated_error;
    double previous_position;
    time_t previous_time;

public:
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
    PIDController(double P, double I, double D, double MAX_RPM,
        double MAX_TORQUE, double TARGET);

    /**
        Destroys a PIDController object
     */
    ~PIDController();


    /**
        
     */
    void start(double current_position);

    /**
        returns: the desired dutyCycle of the PWM signal going to the motor
            ranging from -1 to 1
        note: a negtive sign means the motor should spin in the opposite
            direction
     */
    double dutyCycle(double current_position, double motor_omega);
}

#endif // __PID_Controller_H__