#include "define.h"
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
}

void loop() {
  // put your main code here, to run repeatedly:
  


}

