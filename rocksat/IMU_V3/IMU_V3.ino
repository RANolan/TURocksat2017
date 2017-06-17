
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_L3GD20_U.h"
#include "Adafruit_HMC5883_U.h"
#include <SdFat.h>

int ReadAxis(int);

char filename[20] = "DATA00.CSV";;
SdFat card;
SdFile dataset;

String csvFileHeader = "Time, X Accel, Y Accel, Z Accel, X Gyro, Y Gyro, Z Gyro, X Mag, Y Mag, Z Mag";
String writeData;
char delimiter = ',';

volatile bool dataReady = false;

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

const int xInput = A0;
const int yInput = A1;
const int zInput = A2; 

// Take multiple samples to reduce noise
const int sampleSize = 10;

const int GSwitch = 4;
const int interruptPin = 3;
const int SD_CS = 10;

volatile int i = 0;
volatile int array[36];
volatile int state;
volatile int f;

volatile float xAccel;
volatile float yAccel;
volatile float zAccel;

volatile float xGyro;
volatile float yGyro;
volatile float zGyro;

volatile float xMag;
volatile float yMag;
volatile float zMag;

// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = 509;
int xRawMax = 512;
int xRaw;
 
int yRawMin = 506;
int yRawMax = 514;
int yRaw;
 
int zRawMin = 508;
int zRawMax = 520;
int zRaw;
 
void setup(void) 
{
  analogReference(EXTERNAL);
  
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  SPI.begin();

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

    dataset.open(filename, O_WRITE | O_CREAT | O_AT_END);
  delay(100);
  dataset.println(csvFileHeader);
  dataset.close();

  //delay to get ready for valid data;
  delay(50);

  
//  attachInterrupt(digitalPinToInterrupt(3), readDataStart, RISING);

  Serial.begin(9600);

  while(!readDataStart()){

    
  }
  digitalWrite(GSwitch, HIGH);
  delayMicroseconds(100);
  digitalWrite(GSwitch, LOW);
  attachInterrupt(digitalPinToInterrupt(3), readData, RISING);
  
}

void loop(void) 
{
  digitalWrite(GSwitch, dataReady);
}

void readData()
{
        
        dataReady = false;
        /* Get a new sensor event */ 
        sensors_event_t gyroEvent; 
        sensors_event_t magEvent;
  
        gyro.getEvent(&gyroEvent);
        mag.getEvent(&magEvent);
      
        f = 0;
        
        xRaw = ReadAxis(xInput);
        yRaw = ReadAxis(yInput);
        zRaw = ReadAxis(zInput);

        // re-scale to fractional Gs
        xAccel = (map(xRaw, xRawMin, xRawMax, -1000, 1000)) / 1000.0;
        yAccel = (map(yRaw, yRawMin, yRawMax, -1000, 1000)) / 1000.0;
        zAccel = (map(zRaw, zRawMin, zRawMax, -1000, 1000)) / 1000.0;

      /* Display the results (speed is measured in rad/s) */
      xGyro = gyroEvent.gyro.x;
      yGyro = gyroEvent.gyro.y;
      zGyro = gyroEvent.gyro.z;


      /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
      xMag = magEvent.magnetic.x;
      yMag = magEvent.magnetic.y;
      zMag = magEvent.magnetic.z;

      writeData = String(float(micros()/1000),2) + delimiter + String(xAccel, 4) + delimiter + String(yAccel, 4) + delimiter + String(zAccel, 4) + delimiter + String(xGyro, 4) + delimiter + String(yGyro, 4) + delimiter + String(zGyro, 4) + delimiter + String(xMag, 4) + delimiter + String(yMag, 4) + delimiter + String(zMag, 4); 
      
      
      if(dataset.open(filename, O_WRITE | O_CREAT | O_AT_END)){
      dataset.println(writeData);
      dataset.close();
      // print to the serial port too:
     //Serial.println(writeData);
    }
    else{
      //if card has problem... attempt to reconnect endlessly until it does. Won't get any useful data otherwise.
      while (!card.begin(SD_CS, SPI_FULL_SPEED)) {
      }
    }
        
        dataReady = true;

}

bool readDataStart(){

        bool HighG = false;
        
        dataReady = false;
        /* Get a new sensor event */ 
        sensors_event_t gyroEvent; 
        sensors_event_t magEvent;
  
        gyro.getEvent(&gyroEvent);
        mag.getEvent(&magEvent);
      
        f = 0;
        
        xRaw = ReadAxis(xInput);
        yRaw = ReadAxis(yInput);
        zRaw = ReadAxis(zInput);

        // re-scale to fractional Gs
        xAccel = (map(xRaw, xRawMin, xRawMax, -1000, 1000)) / 1000.0;
        yAccel = (map(yRaw, yRawMin, yRawMax, -1000, 1000)) / 1000.0;
        zAccel = (map(zRaw, zRawMin, zRawMax, -1000, 1000)) / 1000.0;

      /* Display the results (speed is measured in rad/s) */
      xGyro = gyroEvent.gyro.x;
      yGyro = gyroEvent.gyro.y;
      zGyro = gyroEvent.gyro.z;


      /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
      xMag = magEvent.magnetic.x;
      yMag = magEvent.magnetic.y;
      zMag = magEvent.magnetic.z;

      writeData = String(float(micros()/1000),2) + delimiter + String(xAccel, 4) + delimiter + String(yAccel, 4) + delimiter + String(zAccel, 4) + delimiter + String(xGyro, 4) + delimiter + String(yGyro, 4) + delimiter + String(zGyro, 4) + delimiter + String(xMag, 4) + delimiter + String(yMag, 4) + delimiter + String(zMag, 4); 
      
      
      if(dataset.open(filename, O_WRITE | O_CREAT | O_AT_END)){
      dataset.println(writeData);
      dataset.close();
      // print to the serial port too:
     //Serial.println(writeData);
    }
    else{
      //if card has problem... attempt to reconnect endlessly until it does. Won't get any useful data otherwise.
      while (!card.begin(SD_CS, SPI_FULL_SPEED)) {
      }
    }
        
        dataReady = true;

        
        if(xAccel > 5 | yAccel > 5 | zAccel > 5)
          HighG = true;
          
        return HighG;
}

int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int k = 0; k < sampleSize; k++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}


