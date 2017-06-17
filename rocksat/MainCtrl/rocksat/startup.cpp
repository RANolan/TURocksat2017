#include "configuration.h"
#include <Wire.h>
#include <SD.h>
#include <SPI.h>


void setupPinDirections() {

  //SPI
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_SCLK, OUTPUT);

  //I2C
  pinMode(PS_SCL, OUTPUT);
  pinMode(PS_SDA, INPUT_PULLUP);
  pinMode(PS_INT, INPUT);

  //RS232
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);

  //Air Solenoid
  pinMode(AC_SOLENOID, OUTPUT);
  digitalWrite(AC_SOLENOID, LOW);

  //Multipurpose
  pinMode(UV_CLK, OUTPUT);
  pinMode(UV_CONV, OUTPUT);
  pinMode(TEVISO, INPUT);
  pinMode(UV_DATA, INPUT);
  digitalWrite(UV_CLK, LOW);
  digitalWrite(UV_CONV, HIGH);

  //Gerdien Condenser
  pinMode(GC1_CS, OUTPUT);
  pinMode(GC2_CS, OUTPUT);
  pinMode(GC_RST, OUTPUT);
  digitalWrite(GC1_CS, HIGH);
  digitalWrite(GC2_CS, HIGH);
  digitalWrite(GC_RST, LOW);

  //IMU
  pinMode(IMU_GSWITCH, INPUT);
  
  pinMode(IMU_TRIGGER, OUTPUT);
  digitalWrite(IMU_TRIGGER, LOW);

  //SD Card
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
};

//starts the SPI bus
void startSPIBus() {
  SPI.begin();

};

//Starts the I2C bus
void startI2CBUS() {
  Wire.begin();
};

//start the RS232 UART bus
void startRS232Bus() {
  Serial3.begin(38400);
};

//Starts SD card
bool startSDCard(){
  //make sure for SD.begin that the SD_CS is an arguement as it can cause problems elsewhere and the SD card will not work
  return SD.begin(SD_CS);
};


