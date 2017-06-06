#include "configuration.h"
#include "startup.h"
#include "imu.h"
#include "multipurpose.h"
#include "gerdien.h"
#include "pressure.h"
#include "flowmeter.h"
#include "airctrl.h"
#include "sdCard.h"
#include <SD.h>
//
//
//
//
//
//
//  CHECK ALL CSn1 2 5 pins
//
//timer 3 and 4 avaailable
//
//

String csvFileHeader = "Pressure, Temperature, Flow, GC (positive), GC (negative), UV, Radiation, IMU...";
String writeData;
char delimiter = ',';


bool solenoidOpen = false;
bool GSwitchActive = true;
float PS_Data[2];
int i = 0;
unsigned short* IMU;
float radiationData;
char FileName[20] = "DATA00.CSV";;
File dataset;

void setup() {

  setupPinDirections();
  startSPIBus();
  startI2CBUS();
  startRS232Bus();

  Serial.begin(9600); //debugging through arduino
  //reset();
  setup_rad();
  setup_flow();

  while (!startSDCard()) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
   
  }
  
    
  createFile(FileName);
  dataset = SD.open(FileName, FILE_WRITE);
  dataset.println(csvFileHeader);
  dataset.close();
  //digitalWrite(GC_RST, HIGH);
  //delay(1000);
  //digitalWrite(GC_RST, LOW);
}

void loop() {

  if (!solenoidOpen) {
    if (GSwitchActive) {
      openSolenoidForFlight();
      Serial.println("Solenoids Opened");
      solenoidOpen = true;
    }
  }

  //triggerIMUDataCollection();
  getPressureTempData(PS_Data);
  //Serial.println("Pressure");
  //Serial.println(PS_Data[0]);
  //Serial.println("Temperature");
  //Serial.println(PS_Data[1]);
  //Serial.println("UV Raw Data");
  Serial.println(bitBangTheUVBus(), HEX);
  //Serial.println("Reset Gerdien Condender");
  Serial.println("Raw Uncalibrated Positive Ion");
  Serial.println(getGC1Data(), HEX);
  Serial.println("Raw Uncalibrated Negative Ion");
  Serial.println(getGC2Data(), HEX);
  i++;
  if(i > 25){
    Serial.println("Reset GC");
    digitalWrite(GC_RST, HIGH);
    delay(1000);
    digitalWrite(GC_RST, LOW);
    i = 0;
   }
  //digitalWrite(GC_RST, LOW);
  //delay(1000);

  getRadData(&radiationData);
  if ( radiationData > 0) {
    Serial.println("UV");
    Serial.println(radiationData);
  }
  Serial.println("Flow Rate in SLPM");
  Serial.println(get_flowdata());
  //getImuData(IMU);
  //for(int i = 0; ; i ++){
  //    if(IMU[i] != null)
  //}

  writeData = String(PS_Data[0]) + delimiter + String(PS_Data[1]) + delimiter + String(get_flowdata(), 4) + delimiter + String(getGC1Data()) + delimiter + String(getGC2Data()) + delimiter + String(bitBangTheUVBus()) + delimiter + String(radiationData); // imu data at the end

  Serial.println(FileName);
  dataset = SD.open(FileName, FILE_WRITE);
  if (dataset) {
    dataset.println(writeData);
    dataset.close();
    // print to the serial port too:
    Serial.println(writeData);
  }
  else
   Serial.println("SD CArd Issue");

   
  
  delay(1000);


}

