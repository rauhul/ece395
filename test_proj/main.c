#include <stdio.h>
#include <math.h>
#include <rt_misc.h>
#include "LPC11xx.h"

// MPU6050 Core
#include "MPU6050 Core/driver_config.h"
#include "MPU6050 Core/inv_mpu.h"
#include "MPU6050 Core/inv_mpu_dmp_motion_driver.h"
#include "MPU6050 Core/i2c.h"
#include "MPU6050 Core/timer32.h"

extern volatile uint8_t int_data_ready;

// Import external functions from Serial.c file
extern void SER_init (void);

#define DEFAULT_MPU_HZ 4 

	
unsigned short gyro_rate, gyro_fsr;
unsigned char accel_fsr;
float gyro_sens;
unsigned short accel_sens;

void setup_MPU_6050() {
	uint8_t errors = 0;
	printf("Setting up\n");

	struct int_param_s int_param = { 1 };
  errors += mpu_init(&int_param);
  errors += mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);
  errors += mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
  errors += mpu_set_sample_rate(DEFAULT_MPU_HZ);

  errors += mpu_get_sample_rate(&gyro_rate);
	printf("FIFO rate: %d Hz\n", gyro_rate);
	
  errors += mpu_get_gyro_fsr(&gyro_fsr);
	printf("Gyro FSR: +/- %d DPS\n", gyro_fsr);
	
  errors += mpu_get_accel_fsr(&accel_fsr);
	printf("Accel FSR: +/- %d G\n", accel_fsr);

  errors += dmp_load_motion_driver_firmware();
	errors += dmp_enable_feature(DMP_FEATURE_TAP | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL);
  errors += dmp_set_fifo_rate(DEFAULT_MPU_HZ);
	errors += mpu_set_dmp_state(1);
	
  errors += mpu_get_gyro_sens(&gyro_sens);
  errors += mpu_get_accel_sens(&accel_sens);
  printf("%d errors.\n", errors);
}


short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
unsigned long sensor_timestamp;

int main() {
	SER_init();
	
	// Start I2C
  I2CInit(I2CMASTER);

	init_timer32(0, TIME_INTERVAL);
	enable_timer32(0);
	
	delay32Ms(0, 100);
	setup_MPU_6050();

	while(1) {
		if (int_data_ready == 1) {
			dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
			if (more == 1)
				int_data_ready = 0;
			if (sensors & INV_XYZ_GYRO)
				printf("Gyro: %f %f %f\n", gyro[0]/gyro_sens, gyro[1]/gyro_sens, gyro[2]/gyro_sens);
			if (sensors & INV_XYZ_ACCEL)
				printf("Acce: %f %f %f\n", accel[0]/(float)accel_sens, accel[1]/(float)accel_sens, accel[2]/(float)accel_sens);
			printf("\n");
			
			int_data_ready = 0;

		}
	}
}
