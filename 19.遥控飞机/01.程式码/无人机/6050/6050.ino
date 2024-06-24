#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Arduino.h>


//陀螺仪的部份
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
void MPU6050num() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print("陀螺仪:");
  Serial.print("ax:");
  Serial.print(ax / 100);
  Serial.print(", ay:");
  Serial.print(ay / 100);
  Serial.print(", az:");
  Serial.print(az / 100);
  Serial.print(", gx:");
  Serial.print(gx / 10);
  Serial.print(", gy:");
  Serial.print(gy / 10);
  Serial.print(", gz:");
  Serial.println(gz / 10);

  delay(30);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  Wire.begin(4, 5);
  Serial.println("陀螺仪初始化");
  accelgyro.initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  MPU6050num();
  delay(1000);
}
