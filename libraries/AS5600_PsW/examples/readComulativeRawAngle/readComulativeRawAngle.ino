#include "AS5600_PsW.h"

AS5600_PsW Sensor;

void setup() {
  Serial.begin(115200);
  Sensor.init();

  //It is necessary to update and inoke the sensor quickly for cumulative reading
  //for this we need to change power mode and do not let sensor goes to low power mode and sleep
  
  Sensor.changePowerMode(01);//00 = NOM, 01 = LPM1, 10 = LPM2, 11 = LPM3

}

void loop() {
  Serial.println(Sensor.comulativeRawAngle());
  // delay(25);
}
