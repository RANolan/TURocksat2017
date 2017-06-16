#include "configuration.h"
#include <Arduino.h>

#define calFactor 3.4
#define MAXCNT 10

volatile unsigned int rad_pings;
volatile unsigned int oneSecondCounter;
volatile unsigned int fiveSecondCounter; //use array and write each value down the line for sec and minute and average them for dataset
volatile unsigned int oneMinuteCounter;
volatile unsigned int sevenCount;
volatile unsigned long timeStart, timeEnd;
volatile bool ready;

//void radEvent();

unsigned int bitBangTheUVBus(){
  unsigned int data = 0;
  unsigned int temp = 0;
  
  digitalWrite(UV_CONV, LOW);
  delayMicroseconds(1);
  digitalWrite(UV_CONV, HIGH);
  delayMicroseconds(1);

  for(int i = 0; i < 16; i++){
    digitalWrite(UV_CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(UV_CLK, LOW);
    delayMicroseconds(1);
    data |= digitalRead(UV_DATA);
    data = data << 1;
    }

  return data;
};


void setup_rad(){
  pinMode(47, INPUT);
  //attachInterrupt(digitalPinToInterrupt(TEVISO),radEvent, RISING);
  rad_pings = 0;
  //noInterrupts();
  TCCR5A = 0;// set entire TCCR1A register to 0
  TCCR5B = 0;// same for TCCR1B
  TIMSK5 = 0;
  TCNT5  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR5A = MAXCNT;//62500; //23478; //1,5 seconds //62500; //one second   //65535;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR5A |= (1 << WGM52);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR5B |= (1<< CS52)  | (1 << CS51) | (1 << CS50);  
  // enable timer compare interrupt
  TIMSK5 |= (1 << OCIE5A);
  interrupts(); //ENABLE INTERRUPT
  
};

ISR(TIMER5_COMPA_vect){
  timeStart = timeEnd;
  timeEnd = micros();
  //Serial.println("Int Occurred");
  //oneSecondCounter = rad_pings;
  //rad_pings = 0;
  TCNT5 = 0;
  ready = true;
  };

//void radEvent(){
//  rad_pings++;
//  TCNT3++;
//}
//
//


void getRadData(float* data){
  if(!ready)
    {
      *data = -1;
    }
  else
  {
  unsigned long dt;
  //counts/time in seconds / 18.25 for calibration machine calculations... 18 - 18.25
  //float data =  float(((float)oneSecondCounter*60.0/1/calFactor)* 10);  
   dt = timeEnd- timeStart;

   //this float data is in uR... need to divive my 1000 for mRem
   *data = (float)((float)MAXCNT*60.0*10000000.0/(float)dt/calFactor); 
   ready = false;
   //Serial.println(data);
  }
};

