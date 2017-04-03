
#include "Arduino_MPU_6050.h"
#include <Wire.h>
#include <I2Cdev.h>

#ifdef __cplusplus
extern "C" {
#endif
  int8_t i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data)
  {

    return !I2Cdev::writeBytes(slave_addr,reg_addr,length,(uint8_t *)data);
    
    Wire.beginTransmission(slave_addr);
    Wire.write(reg_addr);
    Wire.write(data, length);
    uint8_t ret_val = Wire.endTransmission(true);
    
    if (ret_val) {
      Serial.print(F("i2cWrite failed: "));
      Serial.println(ret_val);
    }  
    return ret_val;
  }

  int8_t i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data)
  {
        return !I2Cdev::readBytes(slave_addr,reg_addr,length,(uint8_t *)data);

    Wire.beginTransmission(slave_addr);
    Wire.write(reg_addr);
    uint8_t ret_val = Wire.endTransmission(true);
    
    if (ret_val) {
      Serial.print(F("i2cRead failed: "));
      Serial.println(ret_val);
      data = NULL;
      return ret_val;
    }
    
    int ret_length = Wire.requestFrom(slave_addr, length);
    if (ret_length != length) {
      Serial.print(F("i2cRequest failed to recieve the number of bytes requested: "));
      Serial.println(ret_val);
      data = NULL;
      return 9;
    }
    
    int index = 0;
    while(Wire.available() && index < length)
    {
      data[index] = Wire.read(); 
    }
  
    if (index != length - 1) {
      Serial.print(F("i2cRequest failed to recieve the number of bytes requested: "));
      Serial.println(ret_val);
      data = NULL;
      return 9;
    }
  
    return 0;
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
  
  


  

