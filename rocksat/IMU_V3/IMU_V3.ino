#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_HMC5883_U.h"
#include "Adafruit_L3GD20_U.h"
#include "Adafruit_LSM303_U.h"
#include <SPI.h>

#define INT_PIN 3
#define SCK_PIN  13
#define MISO_PIN 12
#define SS_PIN   10
int ReadAxis(int);

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


long int count = 1;
uint8_t buffer[3];
uint8_t state = 0;
char buf [100];
volatile byte pos;
volatile boolean process_it;

volatile int xRaw;
volatile int yRaw;
volatile int zRaw;

const int xInput = A0;
const int yInput = A1;
const int zInput = A2; 

volatile float xAccel;
volatile float yAccel;
volatile float zAccel;

volatile float xGyro;
volatile float yGyro;
volatile float zGyro;

volatile float xMag;
volatile float yMag;
volatile float zMag;
volatile float headingDegrees;

// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = 512;
int xRawMax = 538;
 
int yRawMin = 512;
int yRawMax = 538;
 
int zRawMin = 512;
int zRawMax = 538;
 
// Take multiple samples to reduce noise
const int sampleSize = 10;

void setup(void) 
{
  analogReference(EXTERNAL);
  attachInterrupt(INT_PIN, triggered, RISING);
  Serial.begin(9600);
 
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);


  }

  //SPI Setup
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SCK_PIN, INPUT);
  pinMode(SS_PIN, INPUT);

  //SPI.begin();
  //SPI.setClockDivider(SPI_CLOCK_DIV16);
  
  //DDRB=(1<<DDB4);  //prev
  SPCR=(1<<SPE);   //prev
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
    
  // turn on interrupts
  SPCR != _BV(SPIE);
  
  // get ready for an interrupt 
  //pos = 0;   // buffer empty
  //process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();
  
  // interrupt for SS falling edge
  //attachInterrupt (0, ss_falling, FALLING);
  

  
  
}

ISR(SPI_STC_vect){

    byte c = SPDR;
    switch(state){

    case 0:
    SPDR = xRaw & 0xFF;//low byte of xRaw
    Serial.println(SPDR);
    state++;
    break;
    
    case 1:
    SPDR = (xRaw >> 8) & 0xFF;
    Serial.println(SPDR);
    state = 0;
    break;

    /*
    case 2:

    case 3:

    case 4:

    case 5:

    case 6:

    case 7:

    case 8:

    case 9:

    case 10:

    case 11:

    case 12:

    case 13:

    case 14:

    case 15:

    case 16:

    case 17:

    case 18:

    case 19:

    case 20:

    case 21:

    case 22:

    case 23:

    case 24:

    case 25:

    case 26:

    case 27:

    case 28:

    case 29:

    case 30:

    case 31:

    case 32:

    case 33:

    case 34:

    case 35:

    case 36:

    case 37:

    case 38:
    */
    }
}

void loop(void){}

void triggered(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t gyroEvent; 
  sensors_event_t magEvent;
  
  gyro.getEvent(&gyroEvent);
  mag.getEvent(&magEvent);

  xRaw = ReadAxis(xInput);
  yRaw = ReadAxis(yInput);
  zRaw = ReadAxis(zInput);
  
  // re-scale to fractional Gs
  //handle on the arudino mega side
  //xAccel = (map(xRaw, xRawMin, xRawMax, -1000, 1000)) / 1000.0;
  //yAccel = (map(yRaw, yRawMin, yRawMax, -1000, 1000)) / 1000.0;
  //zAccel = (map(zRaw, zRawMin, zRawMax, -1000, 1000)) / 1000.0;
 
  /* Display the results (speed is measured in rad/s) */
  xGyro = gyroEvent.gyro.x;
  yGyro = gyroEvent.gyro.y;
  zGyro = gyroEvent.gyro.z;

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  xMag = magEvent.magnetic.x;
  yMag = magEvent.magnetic.y;
  zMag = magEvent.magnetic.z;
  
  //do on arudino side
  /*
  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(yMag, xMag);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  headingDegrees = heading * 180/M_PI; 

  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  */
  delay(500);
}

//
// Read "sampleSize" samples and report the average
//
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}
