#ifndef __M0_MPU_6050__
#define __M0_MPU_6050__
#if defined MPU6050 & defined ARM_CORTEX_M0

#include "inv_mpu.h"
#include <stdint.h>

#define I2C_BUFFER_RESERVE 2

uint8_t i2c_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);
uint8_t i2c_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);

void delay_ms(unsigned long length);
void get_ms(unsigned long *timestamp);

#define log_i(fmt, ...) printf(fmt, ## __VA_ARGS__)
#define log_e(fmt, ...) printf(fmt, ## __VA_ARGS__)

void reg_int_cb(struct int_param_s *int_param);

#define labs(x)   (((x) >  0 ) ? (x) : (0 - (x)))
#define min(a, b) (((a) > (b)) ? (b) : (a))
#define __no_operation() __nop()

#endif
#endif /* __M0_MPU_6050__ */
