#include "configuration.h"
#include <SPI.h>

void triggerDataCollection(){

  digitalWrite(IMU_TRIGGER, HIGH);
  delayMicroseconds(1);
  digitalWrite(IMU_TRIGGER, LOW);

}


void getImuData(unsigned short* data){

//serial bus data stored into array
  unsigned short comms;
  SPI.beginTransaction(SPISettings(150000, MSBFIRST, SPI_MODE0));
  digitalWrite(IMU_CS, LOW);
  comms = SPI.transfer(0);
  data[0] = comms;
  comms = SPI.transfer(0);
  data[1] = comms;
  comms = SPI.transfer(0);
  data[2] = comms;
  delayMicroseconds(1);
  digitalWrite(IMU_CS, HIGH);
  SPI.endTransaction();
}

