#include "AS5600_PsW.h"

//I2CMagneticSensorConfiguration AS5600 = {
//  .se = 0x36,
//  .bit_resolution = 12,
//  .angle_register = 0x0C,
//  .data_start_bit = 11
//};



void AS5600_PsW::init()
{	
	Wire.begin();
	previousAngle = rawAngle();
}

word AS5600_PsW::readWire(uint8_t _wireRegisterToRead)
{
	uint8_t _MSB{ 0 };
	uint8_t _LSB{ 0 };

	Wire.beginTransmission(sensorI2CAddress);
	Wire.write(_wireRegisterToRead); //invove the sensor
	wireCondition = Wire.endTransmission();

	if (chechWireCondition(wireCondition) == 0) return;

	Wire.requestFrom(sensorI2CAddress, (uint8_t)2);

	//Read Firstly MSB then LSB
	_MSB = Wire.read();
	_LSB = Wire.read();

	//numberOfLsbBits is Writen to include more I2C Sensor Later
	return (_MSB << numberOfLsbBits) | _LSB;
}

bool AS5600_PsW::chechWireCondition(int _wireCondition)
{
	if (_wireCondition != 0) {
		//print sth here
		Serial.print("Error number: ");
		Serial.print(wireCondition);
		Serial.println(" happened in wire transmition of data.");
		Serial.println("1 : data too long to fit in transmit buffer.");
		Serial.println("2 : received NACK on transmit of address.");
		Serial.println("3 : received NACK on transmit of data.");
		Serial.println("4 : other error.");
		Serial.println("5 : timeout");
		delay(10000);
		wireCondition = -1;
		return 0;
	}
	else {
		wireCondition = -1;
		return 1;
	}
}

int AS5600_PsW::rawAngle()
{
	return readWire(rawAngleRegister);
}

float AS5600_PsW::degreeAngle()
{
	return readWire(degreeAngleRegister);
}

int AS5600_PsW::fullRotationUpdate() {

	float _currentAngle = rawAngle();
	float _angleDifference = _currentAngle - previousAngle;
	//if (abs(angleDifference) > (0.8f * () fullRotation += (angleDifference > 0) ? -1 : 1;
	if (abs(_angleDifference) > (0.8f * fullRotationRawCount)) fullRotation += (_angleDifference > 0) ? -1 : 1;

	previousAngle = _currentAngle;
	return fullRotation;
}


//Power Mode
//00 = NOM, 01 = LPM1, 10 = LPM2, 11 = LPM3
void AS5600_PsW::changePowerMode(word _mode) {

	Wire.beginTransmission(sensorI2CAddress); // I2C address of AS5600
	Wire.write(configRegister); // Address of CONF register (0x07)
	Wire.write(0b00000000); // MSB of CONF register (default value)
	Wire.write(_mode); // LSB of CONF register 
	wireCondition = Wire.endTransmission();

	if (chechWireCondition(wireCondition) == 0) return;

}


int AS5600_PsW::comulativeRawAngle()
{
	return (fullRotationUpdate() * fullRotationRawCount) + previousAngle;
}

double AS5600_PsW::comulativeDegreeAngle()
{
	return (comulativeRawAngle() / (double)fullRotationRawCount) * 360.0;
}

double AS5600_PsW::comulativeRadianAngle()
{
	return (comulativeRawAngle() / (double)fullRotationRawCount) * _2PI;
}


