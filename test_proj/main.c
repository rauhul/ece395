#include <stdio.h>
#include <math.h>
#include <rt_misc.h>
#include "LPC11xx.h"

#include "MPU6050 Core/driver_config.h"
#include "MPU6050 Core/inv_mpu.h"
#include "MPU6050 Core/i2c.h"
#include "MPU6050 Core/timer32.h"

extern volatile uint8_t 	I2CMasterBuffer[BUFSIZE], I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t 	I2CReadLength, I2CWriteLength;

// Import external functions from Serial.c file
extern void SER_init (void);

void configureGPIO() {
	//enable clocks to GPIO block
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  16);

	//set port 0_7 to output (high current drain in LPC1114)
  LPC_GPIO0->DIR |= (1<<7);
}

void ledOn() {
	LPC_GPIO0->DATA &= ~(1<<7);
}

void ledOff() {						 
	LPC_GPIO0->DATA |= (1<<7);
}

void delay() {
	int i = 10000000;
	while( i > 0) { i--; }
}

#define DEFAULT_MPU_HZ 4 

volatile uint8_t new_int = 0;
void interrupt(){
   new_int = 1;
}

unsigned short gyro_rate, gyro_fsr;
unsigned char accel_fsr;
float gyro_sens;
unsigned short accel_sens;

void setup_MPU_6050() {
	uint8_t errors = 0;
	printf("Setting up\n");

  struct int_param_s params = { 3, interrupt};
  errors += mpu_init(&params);
//  errors += mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);
//  errors += mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
//  errors += mpu_set_sample_rate(DEFAULT_MPU_HZ);

//  errors += mpu_get_sample_rate(&gyro_rate);
//	printf("FIFOrate: %d Hz\n", gyro_rate);
//	
//  errors += mpu_get_gyro_fsr(&gyro_fsr);
//	printf("Gyro FSR: +/- %d DPS\n", gyro_fsr);
//	
//  errors += mpu_get_accel_fsr(&accel_fsr);
//	printf("Accel FSR: +/- %d G\n", accel_fsr);

//  errors += mpu_get_gyro_sens(&gyro_sens);
//  errors += mpu_get_accel_sens(&accel_sens);
  printf("%d errors.\n", errors);
}

int main() {
	SER_init();
	configureGPIO();
	
	// Start I2C
  I2CInit(I2CMASTER);
	init_timer32(0, TIME_INTERVAL);
	enable_timer32(0);
	
	delay32Ms(0, 100);
	setup_MPU_6050();

	return 0;
}
