#include "configuration.h"
#include <Arduino.h>
#define READ_VAL "9DF00108F90D"
//all the commands can go here for rs232


void setup_flow(){
pinMode(18, OUTPUT);
pinMode(19, INPUT);
//Serial1.begin(38400, SERIAL_8N1, true );
Serial3.begin(38400, SERIAL_8N1, true );

}

bool calibrateFlow(){
  bool didRespond;
  Serial3.write9bit(0x19D);
  Serial3.write9bit(0x72);
  Serial3.write9bit(0x01);
  Serial3.write9bit(0x55);
  Serial3.write9bit(0xC8);
  Serial3.write9bit(0x0D);
  data1 = Serial3.read();
  if(Serial3.available()){
    data1 = Serial3.read();
    data1 = data1 & 0xFF;
    if(data1 = 0x9D)
      didRespond = true;
      didRespond = false; 
  }
  while(Serial3.available() > 0)
      Serial3.read();
  return didRespond;
}

void get_flowdata(){


  // need to remove the extra bit and then calculate
  unsigned long time1, time2;
  Serial.println("getting data");
  time2 = micros();
  int data1, data2, data3, data4, data5, data6, data7, data8;
  Serial3.write9bit(0x19D);
  Serial3.write9bit(0xF0);
  Serial3.write9bit(0x01);
  Serial3.write9bit(0x08);
  Serial3.write9bit(0xF9);
  Serial3.write9bit(0x0D);
  while(Serial3.available()>0){
  data1 = Serial3.read();
  //data5 = Serial2.read();
  //data6 = Serial2.read();
  //data7 = Serial2.read();
  //data8 = Serial2.read();

  //Serial.println(data1, HEX);
  
}
  time1 = micros();

  Serial.println(time1 - time2);
}



