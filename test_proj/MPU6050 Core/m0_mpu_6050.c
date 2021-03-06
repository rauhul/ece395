#include "m0_mpu_6050.h"
#if defined MPU6050 & defined ARM_CORTEX_M0

#include "driver_config.h"
#include "i2c.h"
#include "timer32.h"

#include <stdio.h>

extern volatile uint8_t 	I2CMasterBuffer[BUFSIZE], I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t 	I2CReadLength, I2CWriteLength;


uint8_t i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data) {	
	uint8_t result;
		
	i2c_clear_buffers();
	
	uint8_t write_length = length + I2C_WRITE_BUFFER_RESERVE;
	if (write_length > BUFSIZE)
		return 2;	// Buffer Overflow

	I2CWriteLength      = write_length;
	I2CReadLength		    = 0;
	I2CMasterBuffer[0] 	= slave_addr;
	I2CMasterBuffer[1] 	= reg_addr;
	
	for (uint8_t i = I2C_WRITE_BUFFER_RESERVE; i < write_length; i++) {
			I2CMasterBuffer[i] = data[i - I2C_WRITE_BUFFER_RESERVE];
	}
	
	result = I2CEngine();
	
	#if defined DEBUG
		printf("addr:%d reg: %d\n", slave_addr, reg_addr);
		for (int i = 0; i < length; i++) {
			printf("W: %d\n", data[i]);
		}
	#endif
		
	if(result != I2C_OK)
		return 1; // i2c Engine Failed
	
	#if defined DEBUG
		if (length == 1) {
			uint8_t verify = 123;
			i2c_read(slave_addr, reg_addr, length, &verify);
			if (verify != data[0]){
				printf(">> read != write <<\n");
			}
		}
	#endif

	return 0;
}

uint8_t i2c_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data) {
	uint8_t result;
		
	i2c_clear_buffers();
	
	if (length > BUFSIZE)
		return 2;	// Buffer Overflow
		
	I2CWriteLength      = 2;
	I2CReadLength		    = length;
	I2CMasterBuffer[0] 	= slave_addr;
	I2CMasterBuffer[1] 	= reg_addr;
	I2CMasterBuffer[2] 	= slave_addr | RD_BIT;

	result = I2CEngine();
	
	if(result != I2C_OK)
		return 1; // i2c Engine Failed
	
	for(uint8_t i = 0; i < length; i++) {
		data[i] = I2CSlaveBuffer[i];
	}
	
	#if defined DEBUG
		printf("addr:%d reg: %d\n", slave_addr, reg_addr);
		for (int i = 0; i < length; i++) {
			printf("R: %d\n", data[i]);
		}
	#endif

	return 0;
}

void i2c_clear_buffers() {
	for (uint8_t i = 0; i < BUFSIZE; i++) {
		I2CMasterBuffer[i] = 0;
		I2CSlaveBuffer[i]  = 0;
	}
}

void delay_ms(unsigned long length) {
	delay32Ms(0, length);
}

void get_ms(unsigned long *timestamp) {
	*timestamp = read_timer32(0);
}

#define INT_PIN	(1<<0)	//select pin of GPIO1

uint8_t reg_int_cb(struct int_param_s *int_param) {
	if (int_param->should_setup_int == 0) {
		return 1;
	}
	printf("Attach Interrupt\n");
	
	LPC_IOCON->R_PIO1_0 |= 0x01;  //select GPIO
	LPC_GPIO1->DIR &=~INT_PIN;		//0: input
	LPC_GPIO1->IS  &=~INT_PIN;		//0: Edge on corresponding pin is detected.
	LPC_GPIO1->IBE &=~INT_PIN;		//0: Single edge, determined by corresponding bit in GPIOIEV register.
	LPC_GPIO1->IEV &=~INT_PIN;		//0: Falling edges, or low levels on corresponding pin trigger interrupts.
	LPC_GPIO1->IE  |= INT_PIN;		//1: Corresponding pin interrupt is masked.
	NVIC_EnableIRQ(EINT1_IRQn);

	return 0;
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
#endif
