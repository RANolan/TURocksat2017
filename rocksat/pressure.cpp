#include <Wire.h>
#include "configuration.h"

//takes a pointer to a two element sort array where element 0 conatins two bytes of pressure data (14 bits) and element 1 contains 2 bytes of temperature data (11 bits)
void getPressureTempData(unsigned short* data){
  unsigned short readings;
  Wire.beginTransmission(PS_ADDR);
  
  readings = Wire.read();  // receive high byte (overwrites previous reading)
  readings = readings << 8;    // shift high byte to be high 8 bits
  readings |= Wire.read(); // receive low byte as lower 8 bits
  readings = readings & 0x3FFF; //bitmask for output  
  
  //add conversion here to actual float value and update data type
  
  data[0] = readings;

  readings = Wire.read();  // receive high byte (overwrites previous reading)
  readings = readings << 8;    // shift high byte to be high 8 bits
  readings |= Wire.read(); // receive low byte as lower 8 bits
  readings >> 5;
  readings = readings & 0x07FF; //bitmask for output
  
  //add conversion here to actual float value
  
  data[1] = readings;
   


};
