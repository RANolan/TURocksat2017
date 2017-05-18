#include "configuration.h"
#include <Arduino.h>


//global variable ... wrap in c++ class
volatile int countTimeToStayOpen;

//require logic in front to check when G force of launch has occurred.
//Setup and initialize timer
//then open solenoids for flight and close after specified time
void openSolenoidForFlight(){
  noInterrupts();
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TIMSK1 = 0;
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 65535;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1A |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts(); //ENABLE INTERRUPT
  countTimeToStayOpen = 0;
  digitalWrite(AC_SOLENOID, HIGH);
  
}

ISR(TIMER1_COMPA_vect){
  
  countTimeToStayOpen++;
  if(countTimeToStayOpen == int(ceil((MINUTES_SOLENOID_OPEN * 60)/4.19))){
    Serial.println("Solenoids Closed");
    digitalWrite(AC_SOLENOID, LOW);
    }
  TCNT1 = 0;
}

