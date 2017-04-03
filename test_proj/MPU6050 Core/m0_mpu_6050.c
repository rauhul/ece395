#include "m0_mpu_6050.h"
#if defined MPU6050 & defined ARM_CORTEX_M0

#include "driver_config.h"
#include "i2c.h"
#include "timer32.h"

uint8_t i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data) {	
	uint8_t result;
		
	if (length > BUFSIZE)
		return 2;	// Buffer Overflow
	
	I2CWriteLength      = length;
	I2CReadLength		    = 0;
	I2CMasterBuffer[0] 	= slave_addr;
	I2CMasterBuffer[1] 	= reg_addr;
	// do I need to send ADDR | WRITE
	for (uint8_t i = I2C_BUFFER_RESERVE; i < length; i++) {
			I2CMasterBuffer[i] = data[i - I2C_BUFFER_RESERVE];
	}
	
	result = I2CEngine();
	if(result != I2C_OK)
		return 1; // i2c Engine Failed

	return 0;
}

uint8_t i2c_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data) {
	uint8_t result;
		
	if (length > BUFSIZE)
		return 2;	// Buffer Overflow
	
	I2CWriteLength      = 0;
	I2CReadLength		    = length;
	I2CMasterBuffer[0] 	= slave_addr;
	I2CMasterBuffer[1] 	= reg_addr;
	// do I need to send ADDR | READ

	result = I2CEngine();
	if(result != I2C_OK)
		return 1; // i2c Engine Failed
	
	for(uint8_t i = 0; i < length; i++) {
		data[i] = I2CSlaveBuffer[i];
	}

	return 0;
}

void delay_ms(unsigned long length) {
	delay32Ms(0, length);
}

void get_ms(unsigned long *timestamp) {
	*timestamp = read_timer32(0);
}

void reg_int_cb(struct int_param_s *int_param) {
	// attach interrupt callback to pin described in int_param
}

#endif
