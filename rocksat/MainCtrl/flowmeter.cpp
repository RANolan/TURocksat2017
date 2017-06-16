#include "configuration.h"
#include <Arduino.h>
#define READ_VAL "9DF00108F90D"
//all the commands can go here for rs232


void setup_flow() {
  pinMode(18, OUTPUT);
  pinMode(19, INPUT);
  //Serial1.begin(38400, SERIAL_8N1, true );
  Serial3.begin(38400, SERIAL_8N1, true );

}

bool calibrateFlow() {
  long int data1;
  bool didRespond;
  Serial3.write9bit(0x19D);
  Serial3.write9bit(0x72);
  Serial3.write9bit(0x01);
  Serial3.write9bit(0x55);
  Serial3.write9bit(0xC8);
  Serial3.write9bit(0x0D);
  data1 = Serial3.read();
  if (Serial3.available()) {
    data1 = Serial3.read();
    data1 = data1 & 0xFF;
    if (data1 = 0x9D)
      didRespond = true;
    didRespond = false;
  }
  while (Serial3.available() > 0)
    Serial3.read();
  return didRespond;
}

float get_flowdata() {

  // need to remove the extra bit and then calculate
  //Serial.println("getting data");

  long int FRH, FRM, FRL;
  uint16_t i = 0;
  float flowResult;

  Serial3.write9bit(0x19D);
  Serial3.write9bit(0xF0);
  Serial3.write9bit(0x01);
  Serial3.write9bit(0x08);
  Serial3.write9bit(0xF9);
  Serial3.write9bit(0x0D);
  //delay(10);
  if (Serial3.available() == 8) {
    Serial3.read();
    Serial3.read();
    Serial3.read();
    FRH = Serial3.read() & 0xFF;
    FRM = Serial3.read() & 0xFF;
    FRL = Serial3.read() & 0xFF;
    Serial3.read();
    Serial3.read();
    flowResult = float(float((FRH * 65536) +  (FRM * 256) + FRL) / 1000);
  }

  else
    flowResult = -1000;
  //data1 = Serial3.read();
  //data5 = Serial2.read();
  //data6 = Serial2.read();
  //data7 = Serial2.read();
  //data8 = Serial2.read();

  //Serial.println(data1, HEX);

  return flowResult;

}



