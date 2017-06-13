#include "configuration.h"
#include "startup.h"
#include "imu.h"
#include "multipurpose.h"
#include "gerdien.h"
#include "pressure.h"
#include "flowmeter.h"
#include "airctrl.h"
#include "sdCard.h"
#include <SdFat.h>
//
//
//
//
//  add CD pin and reopen file system to write to file ... 
//    ADD timestamps
//  CHECK ALL CSn1 2 5 pins
//
//timer 3 and 4 avaailable
//
//

String csvFileHeader = "Time From Power, Pressure, Temperature, Flow, GC (negative), GC (positive), UV, Radiation, IMU...";
String writeData;
char delimiter = ',';


//bool solenoidOpen = false;
//bool GSwitchActive = true;
//float PS_Data[2];
//byte IMU[4];
//int i = 0;
////unsigned short* IMU;
//float radiationData;
char filename[20] = "DATA00.CSV";;
SdFat card;
SdFile dataset;

void setup() {

  setupPinDirections();
  startSPIBus();
//  startI2CBUS();
//  startRS232Bus();

  Serial.begin(9600); //debugging through arduino
  //reset();
//  setup_rad();
//  setup_flow();
  

  while (!card.begin(SD_CS, SPI_FULL_SPEED)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
   
  }
  
    for (uint8_t i = 0; i < 100; i++) {
      filename[4] = i/10 + '0';
      filename[5] = i%10 + '0';
      if (!card.exists(filename)) {
        // only open a new file if it doesn't exist
        break;  // leave the loop!
    }
    }

    //open, write the header and close
  dataset.open(filename, O_WRITE | O_CREAT | O_AT_END);
  dataset.println(csvFileHeader);
  dataset.close();

  //delay to get ready for valid data;
  delay(50);
  
}



void loop() {
// 
//  if (!solenoidOpen) {
//    if (GSwitchActive) {
//      openSolenoidForFlight();
//      Serial.println("Solenoids Opened");
//      solenoidOpen = true;
//    }
//  }

  //triggerIMUDataCollection();
 // getPressureTempData(PS_Data);
  //Serial.println("Pressure");
  //Serial.println(PS_Data[0]);
  //Serial.println("Temperature");
  //Serial.println(PS_Data[1]);
  //Serial.println("UV Raw Data");
//  Serial.println(bitBangTheUVBus(), HEX);
//  //Serial.println("Reset Gerdien Condender");
//  Serial.println("Raw Uncalibrated Positive Ion");
//  Serial.println(getGC1Data(), HEX);
//  Serial.println("Raw Uncalibrated Negative Ion");
//  Serial.println(getGC2Data(), HEX);
//  i++;
//  if(i > 25){
//    Serial.println("Reset GC");
//    digitalWrite(GC_RST, HIGH);
//    delay(1000);
//    digitalWrite(GC_RST, LOW);
//    i = 0;
//   }
  //digitalWrite(GC_RST, LOW);
  //delay(1000);
//
//  getRadData(&radiationData);
//  if ( radiationData > 0) {
//    Serial.println("UV");
//    Serial.println(radiationData);
//  }
//  Serial.println("Flow Rate in SLPM");
//  Serial.println(get_flowdata());
  //getImuData(IMU);
  //for(int i = 0; ; i ++){
  //    if(IMU[i] != null)
  //}
  digitalWrite(GC_RST, HIGH);
//Serial.println(getGC1Data(), HEX);
//Serial.println(getGC2Data(), HEX);

  
  //writeData = String(micros()) + delimiter + String(PS_Data[0]) + delimiter + String(PS_Data[1]) + delimiter + String(get_flowdata(), 4) + delimiter + String(getGC1Data()) + delimiter + String(getGC2Data()) + delimiter + String(bitBangTheUVBus()) + delimiter + String(radiationData); // imu data at the end
  writeData = String(getGC1Data(), HEX) + delimiter + String(getGC2Data(), HEX);
  //Serial.println(csvFileHeader);
  //Serial.println(writeData);
//  Serial.println(filename);
//    digitalWrite(SD_CS, LOW);
  if(dataset.open(filename, O_WRITE | O_CREAT | O_AT_END)){
    dataset.println(writeData);
    dataset.close();
      digitalWrite(SD_CS, HIGH);
    // print to the serial port too:
   Serial.println(writeData);
  }
  else{
    //if card has problem... attempt to reconnect endlessly until it does. Won't get any useful data otherwise.
    while (!card.begin(SD_CS, SPI_FULL_SPEED)) {
    }
   
  }



}

