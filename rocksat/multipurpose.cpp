#include "configuration.h"
#include <Arduino.h>



unsigned int bitBangTheBus(){
  unsigned int data = 0;
  unsigned int temp = 0;
  
  digitalWrite(UV_CONV, LOW);
  delayMicroseconds(1);
  digitalWrite(UV_CONV, HIGH);
  delayMicroseconds(1);

  for(int i = 0; i < 16; i++){
    digitalWrite(UV_CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(UV_CLK, LOW);
    delayMicroseconds(1);
    data |= digitalRead(UV_DATA);
    data = data << 1;
    }

  return data;
};


unsigned int getRadData(){

  unsigned int data = 0;
   


  return data;
};

