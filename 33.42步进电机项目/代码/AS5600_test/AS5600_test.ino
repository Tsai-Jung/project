//
//    FILE: AS5600_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//    DATE: 2022-05-28

#include "Wire.h"
#include "AS5600.h"


AS5600 as5600;   //  use default Wire


void setup()
{
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.print("AS5600_LIB_VERSION: ");
  Serial.println(AS5600_LIB_VERSION);
  Wire.begin(4,5);  //  前面4(SDA) 后面5(SCL)
  as5600.begin(); 
  as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
  int b = as5600.isConnected();
  Serial.print("Connect: ");
  Serial.println(b);
  delay(1000);
}


void loop()
{
  Serial.print(as5600.readAngle());
  delay(1000);
}
