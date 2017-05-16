

/*
Pin defintions for all pins usued
17 pins total
*/

#ifndef STARTUP_H_   /* Include guard */
#define STARTUP_H_

#include "Arduino.h"
//function to set pin directions initially
void setupPinDirections();

//starts the SPI bus
void startSPIBus();

//Starts the I2C bus
void startI2CBUS();

//start the RS232 UART bus
void startRS232Bus();

void startSDCard();
#endif



