#include "configuration.h"
#include "startup.h"
#include "imu.h"
#include "multipurpose.h"
#include "gerdien.h"
#include "pressure.h"
#include "flowmeter.h"
#include "airctrl.h"

void setup() {
  setupPinDirections();
  startSPIBus();
  startI2CBUS();
  startRS232Bus();
  startSDCard();
  Serial.begin(9600); //debugging through arduino
}

void loop() {
  bool solenoidOpen = false;
  bool GSwitchActive = true;

  
  if(!solenoidOpen){
      if(GSwitchActive){
        openSolenoidForFlight();
        solenoidOpen = true;
    }
  }
 
  


}

