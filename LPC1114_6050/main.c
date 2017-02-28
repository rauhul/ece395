#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "i2c.h"
#include "mpu6050.h"
#include "timer32.h"

int main() {
    // Start I2C
    I2CInit(I2CMASTER);
    printf("Initializing MPU6050\n");
    int i = 1000000000;
    while(MPU6050_init() != 0)
	{
        while( i > 0) {
            i--;
            // twiddle thumbs
        }
	}
    MPU6050_setZero();
    printf("MPU6050 Ready!\n");

    float acc_x, acc_z, gyro_x;
	float acc_angle, kal_angle, dt;

    i = 0;
    init_timer32(0, 48);
    enable_timer32(0);

    while(1) {
        gyro_x 	= 	MPU6050_getGyroRoll_degree();
        acc_x 	=   MPU6050_getAccel_x();
        acc_z 	= 	-MPU6050_getAccel_z();
        acc_angle = atan2(acc_x , -acc_z) * 180/3.14159265358979323;
        printf("G ROLL  %f\nACC   X %f\nACC   Z %f\nACC ANG %f\n", gyro_x, acc_x, acc_z, acc_angle);
    }
}
