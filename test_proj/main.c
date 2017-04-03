#include <stdio.h>
#include <math.h>
#include <rt_misc.h>
#include "LPC11xx.h"

#include "MPU6050 Core/i2c.h"

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

int main() {
	SER_init();
	configureGPIO();

	// Start I2C
  // I2CInit(I2CMASTER);
	

	return 0;
}
