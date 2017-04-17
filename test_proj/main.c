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

#define INT_PIN	(1<<0)	//select pin of GPIO1

void configureGPIO() {
	//enable clocks to GPIO block
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  16);
	
	LPC_IOCON->R_PIO1_0 |= 0x01;  //select GPIO
	LPC_GPIO1->DIR &=~INT_PIN;		//0: input
	LPC_GPIO1->IS  &=~INT_PIN;		//0: Edge on corresponding pin is detected.
	LPC_GPIO1->IBE &=~INT_PIN;		//0: Single edge, determined by corresponding bit in GPIOIEV register.
	LPC_GPIO1->IEV &=~INT_PIN;		//0: Falling edges, or low levels on corresponding pin trigger interrupts.
	LPC_GPIO1->IE  |= INT_PIN;		//1: Corresponding pin interrupt is masked.
	NVIC_EnableIRQ(EINT1_IRQn);
}

//PIO1 interrupt handler
volatile int int_data_ready = 0;
void PIOINT1_IRQHandler(void) {
	if(LPC_GPIO1->RIS & INT_PIN) { //read raw interrupt status
		int_data_ready = 1;
		LPC_GPIO1->IC |= INT_PIN;
		__NOP();
		__NOP();
		//clear edge detection
	}
}

#define DEFAULT_MPU_HZ 4 

unsigned short gyro_rate, gyro_fsr;
unsigned char accel_fsr;
float gyro_sens;
unsigned short accel_sens;

void setup_MPU_6050() {
	uint8_t errors = 0;
	printf("Setting up\n");

  errors += mpu_init(NULL);
  errors += mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);
  errors += mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
  errors += mpu_set_sample_rate(DEFAULT_MPU_HZ);

  errors += mpu_get_sample_rate(&gyro_rate);
	printf("FIFO rate: %d Hz\n", gyro_rate);
	
  errors += mpu_get_gyro_fsr(&gyro_fsr);
	printf("Gyro FSR: +/- %d DPS\n", gyro_fsr);
	
  errors += mpu_get_accel_fsr(&accel_fsr);
	printf("Accel FSR: +/- %d G\n", accel_fsr);

//	printf("result: %d\n", dmp_load_motion_driver_firmware());
//  errors += dmp_enable_feature(DMP_FEATURE_TAP | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL);
//  errors += dmp_set_fifo_rate(DEFAULT_MPU_HZ);
//  errors += mpu_set_dmp_state(1);
	
  errors += mpu_get_gyro_sens(&gyro_sens);
  errors += mpu_get_accel_sens(&accel_sens);
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
		
	while(1) {
		if (int_data_ready == 1) {
			int_data_ready = 0;
			printf("DATA READY TO CONSUME\n");
		}
	}
	
	return 0;
}
