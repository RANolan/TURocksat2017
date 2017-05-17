#include "configuration.h"
#include <Arduino.h>


//global variable ... wrap in c++ class
int countTimeToStayOpen = 0;

//require logic in front to check when G force of launch has occurred.
//Setup and initialize timer
//then open solenoids for flight and close after specified time
void openSolenoidForFlight(){
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 65535;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts(); //ENABLE INTERRUPT
  digitalWrite(AC_SOLENOID, HIGH);
  
};

ISR(TIMER1_COMPA_vect){
  
  countTimeToStayOpen++;
  if(countTimeToStayOpen == int(((MINUTES_SOLENOID_OPEN * 60)/4.19)))
    digitalWrite(AC_SOLENOID, LOW);
};

