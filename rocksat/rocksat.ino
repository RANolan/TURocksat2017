#include "configuration.h"
#include "startup.h"
#include "imu.h"
#include "multipurpose.h"
#include "gerdien.h"
#include "pressure.h"
#include "flowmeter.h"
#include "airctrl.h"

  bool solenoidOpen = false;
  bool GSwitchActive = true;
  float PS_Data[2];
void setup() {


  
  setupPinDirections();
  startSPIBus();
  startI2CBUS();
  startRS232Bus();
  startSDCard();
  Serial.begin(9600); //debugging through arduino
}

void loop() {


  
  if(!solenoidOpen){
      if(GSwitchActive){
        openSolenoidForFlight();
        Serial.println("Solenoids Opened");
        solenoidOpen = true;
    }
  }
 
  getPressureTempData(PS_Data);
  Serial.println("Pressure");
  Serial.println(PS_Data[0]);
  Serial.println("Temperature");
  Serial.println(PS_Data[1]);
  Serial.println("UV Raw Data");
  Serial.println(bitBangTheBus(), HEX);
  
  delay(1000);


}

