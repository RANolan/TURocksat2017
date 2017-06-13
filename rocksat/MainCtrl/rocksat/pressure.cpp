#include <Wire.h>
#include "configuration.h"
#include <Arduino.h>

//takes a pointer to a two element sort array where element 0 conatins two bytes of pressure data (14 bits) and element 1 contains 2 bytes of temperature data (11 bits)
void getPressureTempData(float* data){
  unsigned short readings;

  //pressure
  Wire.beginTransmission(PS_ADDR);
  Wire.requestFrom(PS_ADDR, 4);
  readings = Wire.read();  // receive high byte (overwrites previous reading)
  readings = readings << 8;    // shift high byte to be high 8 bits
  readings |= Wire.read(); // receive low byte as lower 8 bits
  readings = readings & 0x3FFF; //bitmask for output  
  //Serial.println("Pressure valid");
  //Serial.println(readings);
  //add conversion here to actual float value and update data type
  //DEBUG -10 for pressure
  Serial.println("Raw Pressure");
  Serial.println(readings,HEX);
  //this is pressure for differential
  data[0] = float(-1 * (((float(float(readings) - 1628.3) * 24)/float(16383 * 0.80))) - 12);
  
  //this is pressure for 0-100 single port
  //data[0] = float((((float(float(readings) - 1628.3) * 80)/float(16383 * 0.80))) + 10);
  
  readings = 0;
//temperature
  readings = Wire.read();  // receive high byte (overwrites previous reading)
  readings = readings << 8;    // shift high byte to be high 8 bits
  readings |= Wire.read(); // receive low byte as lower 8 bits
  readings = readings >> 5;
  //Serial.println("Temperature Valid");
  //Serial.println(readings);
  //add conversion here to actual float value
  data[1] = float( (float(float(readings) * 200)/2047) - 50);


};
