
#include "Arduino_MPU_6050.h"
#include <Wire.h>
#include <I2Cdev.h>

#ifdef __cplusplus
extern "C" {
#endif
  int8_t i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data)
  {
    int8_t retval = !I2Cdev::writeBytes(slave_addr,reg_addr,length,(uint8_t *)data);
    Serial.print("addr:"); Serial.print(slave_addr); Serial.print(" reg: "); Serial.println(reg_addr);
    for (int i = 0; i < length; i++) {
      Serial.print("W: "); Serial.println(data[i]);
    }

    if (length == 1) {
      uint8_t verify = 10;
      i2c_read(slave_addr, reg_addr, length, &verify);
      if (verify != data[0]){
        Serial.println(">> read != write <<");
      }
    }

    return retval;
  }

  int8_t i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data)
  {
    int8_t retval = !I2Cdev::readBytes(slave_addr,reg_addr,length,(uint8_t *)data);
    Serial.print("addr:"); Serial.print(slave_addr); Serial.print(" reg: "); Serial.println(reg_addr);
    for (int i = 0; i < length; i++) {
      Serial.print("R: "); Serial.println(data[i]);
    }
    return retval;
  }

  int8_t reg_int_cb(struct int_param_s *int_param)
  {
    Serial.println("Setting interrupt callback");
    pinMode(int_param->pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(int_param->pin), int_param->cb, FALLING);
    return 0;
  }

  
#ifdef __cplusplus
}
#endif
  
  


  

