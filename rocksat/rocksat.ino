#include "configuration.h"
#include "startup.h"
#include "imu.h"
#include "multipurpose.h"
#include "gerdien.h"
#include "pressure.h"
#include "flowmeter.h"
#include "airctrl.h"
//
//
//
//
//
//
//  CHECK ALL CSn1 2 3 pins
//
//
//
  bool solenoidOpen = false;
  bool GSwitchActive = true;
  float PS_Data[2];
  int i = 0;
  unsigned short* IMU;
  float radiationData;
  
void setup() {
  
  setupPinDirections();
  startSPIBus();
  startI2CBUS();
  startRS232Bus();
  //startSDCard();
  Serial.begin(9600); //debugging through arduino
  //reset();
  setup_rad();
  setup_flow();
  //digitalWrite(GC_RST, HIGH);
  //delay(1000);
  //digitalWrite(GC_RST, LOW);
  }

void loop() {
  
  if(!solenoidOpen){
      if(GSwitchActive){
        openSolenoidForFlight();
        Serial.println("Solenoids Opened");
        solenoidOpen = true;
    }
  }
  
  //triggerIMUDataCollection();
  getPressureTempData(PS_Data);
  Serial.println("Pressure");
  Serial.println(PS_Data[0]);
  Serial.println("Temperature");
  Serial.println(PS_Data[1]);
  Serial.println("UV Raw Data");
  Serial.println(bitBangTheUVBus(), HEX);
  //Serial.println("Reset Gerdien Condender");
  //Serial.println("Raw Uncalibrated Positive Ion");
  //Serial.println(getGC1Data(), HEX);
  //Serial.println("Raw Uncalibrated Negative Ion");
  //Serial.println(getGC2Data(), HEX);
  //i++;
  //if(i > 25){
  //  digitalWrite(GC_RST, HIGH);
  //  delay(1000);
  //  digitalWrite(GC_RST, LOW);
  //  i = 0;
 // }
  //digitalWrite(GC_RST, LOW);
  //delay(1000);
  
  getRadData(&radiationData);
  if( radiationData > 0){
  Serial.println("UV");
  Serial.println(radiationData);
  }
  get_flowdata();
  //getImuData(IMU);
  //for(int i = 0; ; i ++){
  //    if(IMU[i] != null)
  //      Serial.println(IMU);
    
  //}
  
  delay(1000);


}

