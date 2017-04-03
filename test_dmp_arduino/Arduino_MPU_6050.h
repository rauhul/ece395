#ifndef __ARDUINO_MPU_6050__
#define __ARDUINO_MPU_6050__

#include <Arduino.h>
#include "inv_mpu.h"
#define TARGET_ARDUINO_UNO
#define MPU_6050

#ifdef __cplusplus
extern "C" {
#endif
  int8_t i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data);
  int8_t i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data);
  int8_t reg_int_cb(struct int_param_s *int_param);

#ifdef __cplusplus
}
#endif

static inline void get_ms(unsigned long *timestamp)
{
  *timestamp = millis();  
}

#define delay_ms delay
#define log_i(...) while(0);
#define log_e(message) Serial.print(message)

#endif // __ARDUINO_MPU_6050__
