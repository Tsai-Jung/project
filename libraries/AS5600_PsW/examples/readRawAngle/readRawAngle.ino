#include "AS5600_PsW.h"

AS5600_PsW Sensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Sensor.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(Sensor.rawAngle());
  delay(25);
}
