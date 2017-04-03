#include "Arduino_MPU_6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#define DEFAULT_MPU_HZ 4 

volatile bool new_int = false;
void interrupt(){
   new_int = true;
//  Serial.println("Interrupted");
}

unsigned short gyro_rate, gyro_fsr;
unsigned char accel_fsr;


float gyro_sens;
unsigned short accel_sens;
void setup() {
  uint8_t errors = 0;
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Setting up");
  struct int_param_s params = { interrupt, 3 };
  errors += mpu_init(&params);
  errors += mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);
  errors += mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
  errors += mpu_set_sample_rate(DEFAULT_MPU_HZ);

  errors += mpu_get_sample_rate(&gyro_rate); Serial.print("FIFOrate: ");Serial.print(gyro_rate);Serial.println("Hz");
  errors += mpu_get_gyro_fsr(&gyro_fsr); Serial.print("Gyro FSR: +/- ");Serial.print(gyro_fsr);Serial.println("DPS");
  errors += mpu_get_accel_fsr(&accel_fsr); Serial.print("Accel FSR: +/- ");Serial.print(accel_fsr);Serial.println("G");

  Serial.println(dmp_load_motion_driver_firmware());
  Serial.println(dmp_enable_feature(DMP_FEATURE_TAP | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL));
  Serial.println(dmp_set_fifo_rate(DEFAULT_MPU_HZ));
  Serial.println(mpu_set_dmp_state(1));


  errors += mpu_get_gyro_sens(&gyro_sens);
  errors += mpu_get_accel_sens(&accel_sens);
  Serial.print(errors); Serial.println(" errors.");

}

short gyro[3], accel[3];
unsigned char sensors;
void loop() {
  if(new_int){
    mpu_get_gyro_reg(gyro, NULL); mpu_get_accel_reg(accel, NULL);
    if(sensors & INV_XYZ_GYRO)
      Serial.print("Gyro: ");Serial.print(gyro[0]/gyro_sens);Serial.print(" ");Serial.print(gyro[1]/gyro_sens);Serial.print(" ");Serial.println(gyro[2]/gyro_sens);
    if(sensors & INV_XYZ_ACCEL)
      Serial.print("Acce: ");Serial.print(accel[0]/(float)accel_sens);Serial.print(" ");Serial.print(accel[1]/(float)accel_sens);Serial.print(" ");Serial.println(accel[2]/(float)accel_sens);
    Serial.println();
    new_int = false;
  }
}
