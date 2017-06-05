#include "configuration.h"
#include <SPI.h>

int countUntilGCReset = 0;


unsigned long getGC1Data(){
  unsigned long data;
  SPI.beginTransaction(SPISettings(150000, MSBFIRST, SPI_MODE0));
  digitalWrite(GC1_CS, LOW);
  data = SPI.transfer(0);
  data = data << 8;
  data |= SPI.transfer(0);
  data = data >> 3;
  data = data & 0x0FFF;
  //data |= SPI.transfer(0);
  //data << 8;
  //data |= SPI.transfer(0);
  delayMicroseconds(1);
  digitalWrite(GC1_CS, HIGH);
  SPI.endTransaction();
  return data;
 
};
unsigned long getGC2Data(){
  unsigned long data;
  SPI.beginTransaction(SPISettings(150000, MSBFIRST, SPI_MODE0));
  digitalWrite(GC2_CS, LOW);
  data = SPI.transfer(0);
  data = data << 8;
  data |= SPI.transfer(0);
  data = data >> 3;
  data = data & 0x0FFF;
  
  //data = SPI.transfer(0);
  //data << 8;
  //data = SPI.transfer(0);
  delayMicroseconds(1);
  digitalWrite(GC2_CS, HIGH);
  SPI.endTransaction();
  return data;

  
};



//setup timer and use to reset gerdien condenser
void reset(){

  TCCR2A = 0;// set entire TCCR1A register to 0
  // set compare match register for 1hz increments
  OCR2A = 255;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR2A |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR2B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  interrupts(); //ENABLE INTERRUPT
  digitalWrite(GC_RST, HIGH);
  delayMicroseconds(1);
  digitalWrite(GC_RST, LOW);
}

ISR(TIMER2_COMPA_vect){
  countUntilGCReset++;
  if(countUntilGCReset == int(float((SECONDS_UNTIL_GC_RESET)/float(0.01632)))){
    digitalWrite(GC_RST, HIGH);
    //delay for a little bit
    delayMicroseconds(10);
    digitalWrite(GC_RST, LOW);
  }
};
