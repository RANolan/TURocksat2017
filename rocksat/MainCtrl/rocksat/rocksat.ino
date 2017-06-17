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

String csvFileHeader = "Time From Power, Pressure, Temperature, Flow, GC (negative), GC (positive), UV, Radiation, IMU";
String writeData;
char delimiter = ',';


volatile bool ImuData = false;
bool solenoidOpen = false;
volatile bool GSwitchActive = false;
float PS_Data[2];
byte IMU[4];
int i = 0;
//unsigned short* IMU;
float radiationData;
char filename[20] = "DATA00.CSV";;
SdFat card;
SdFile dataset;



void setup() {

  setupPinDirections();
  startSPIBus();
  startI2CBUS();
  startRS232Bus();

  Serial.begin(9600); //debugging through arduino
  reset();
  setup_rad();
  setup_flow();
  

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
  delay(100);
  dataset.println(csvFileHeader);
  dataset.close();

  //delay to get ready for valid data;
  delay(50);

  attachInterrupt(digitalPinToInterrupt(IMU_GSWITCH), startData, RISING);

    while(!GSwitchActive)
    {
      detachInterrupt(digitalPinToInterrupt(IMU_GSWITCH));
      delayMicroseconds(5);
       attachInterrupt(digitalPinToInterrupt(IMU_GSWITCH), imu_data, RISING);
    }
  
}



void loop() {

  
  if (!solenoidOpen)
    {
      openSolenoidForFlight();
      Serial.println("Solenoids Opened");
      solenoidOpen = true;
    
    }

    triggerIMUDataCollection();
    getPressureTempData(PS_Data);

    getRadData(&radiationData);
    
    writeData = String(float(micros()/1000), 2) + delimiter + String(PS_Data[0]) + delimiter + String(PS_Data[1]) + delimiter + String(get_flowdata(), 4) + delimiter + String(getGC1Data()) + delimiter + String(getGC2Data()) + delimiter + String(bitBangTheUVBus()) + delimiter + String(radiationData) + delimiter + String(ImuData);; // imu data at the end
    ImuData = false;
    if(dataset.open(filename, O_WRITE | O_CREAT | O_AT_END)){
      dataset.println(writeData);
      dataset.close();
        digitalWrite(SD_CS, HIGH);
      // print to the serial port too:
     //Serial.println(writeData);
    }
    else{
      //if card has problem... attempt to reconnect endlessly until it does. Won't get any useful data otherwise.
      while (!card.begin(SD_CS, SPI_FULL_SPEED)) {
      }
    }
  
  }



void startData (){

GSwitchActive = true;
}


void imu_data(){

  ImuData = true;

}


