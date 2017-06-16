
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_HMC5883_U.h>

int ReadAxis(int);

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

typedef union
{
  float number;
  uint8_t b[4];
}FLOATUNION_t;

const int xInput = A0;
const int yInput = A1;
const int zInput = A2; 

// Take multiple samples to reduce noise
const int sampleSize = 10;

const int interruptPin = 3;
volatile int i = 0;
volatile int array[4];
volatile int state;
volatile int f;

volatile FLOATUNION_t xAccel;
volatile FLOATUNION_t yAccel;
volatile FLOATUNION_t zAccel;

volatile FLOATUNION_t xGyro;
volatile FLOATUNION_t yGyro;
volatile FLOATUNION_t zGyro;

volatile FLOATUNION_t xMag;
volatile FLOATUNION_t yMag;
volatile FLOATUNION_t zMag;

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
  
  // have to send on master in, *slave out*
  pinMode(MISO, INPUT);
  pinMode(interruptPin, INPUT);
  state = 0;

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

   // turn on interrupts
  SPCR |= _BV(SPIE);

  attachInterrupt(digitalPinToInterrupt(3), readData, RISING);

  Serial.begin(9600);
  
}

void loop(void) 
{
  /*does nothing*/
}

void readData()
{

  switch(state)
  {
    case 0: 
//   Serial.println("C0");
//        /* Get a new sensor event */ 
//        sensors_event_t gyroEvent; 
//        sensors_event_t magEvent;
//  
//        gyro.getEvent(&gyroEvent);
//        mag.getEvent(&magEvent);
      
        f = 0;
        
        xRaw = ReadAxis(xInput);
//        yRaw = ReadAxis(yInput);
//        zRaw = ReadAxis(zInput);

        // re-scale to fractional Gs
        xAccel.number = (map(xRaw, xRawMin, xRawMax, -1000, 1000)) / 1000.0;
//        yAccel.number = (map(yRaw, yRawMin, yRawMax, -1000, 1000)) / 1000.0;
//        zAccel.number = (map(zRaw, zRawMin, zRawMax, -1000, 1000)) / 1000.0;

        for(int j = 3; j >= 0; j--)
        {
          array[f] = xAccel.b[j];
          f++;
        }
          
//        for(int j = 3; j >= 0; j--)
//        {
//          array[f] = yAccel.b[j];
//          f++;
//        }
//
//        for(int j = 3; j >= 0; j--)
//        {
//          array[f] = zAccel.b[j];
//          f++;
//        }

//      /* Display the results (speed is measured in rad/s) */
//      xGyro.number = gyroEvent.gyro.x;
//      yGyro.number = gyroEvent.gyro.y;
//      zGyro.number = gyroEvent.gyro.z;

//      for(int j = 3; j >= 0; j--)
//        {
//          array[f] = xGyro.b[j];
//          f++;
//        }
//
//        for(int j = 3; j >= 0; j--)
//        {
//          array[f] = yGyro.b[j];
//          f++;
//        }
//
//        for(int j = 3; j >= 0; j--)
//        {
//          array[f] = zGyro.b[j];
//          f++;
//        }

//      /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
//      xMag.number = magEvent.magnetic.x;
//      yMag.number = magEvent.magnetic.y;
//      zMag.number = magEvent.magnetic.z;

//      for(int j = 3; j >= 0; j--)
//        {
//          array[f] = xMag.b[j];
//          f++;
//        }
//
//        for(int j = 3; j >= 0; j--)
//        {
//          array[f] = yMag.b[j];
//          f++;
//        }
//
//        for(int j = 3; j >= 0; j--)
//        {
//          array[f] = zMag.b[j];
//          f++;
//        }
        
        i = 1;
        state = 1;
        break;
        
    case 1:
        pinMode(MISO,INPUT);
        state = 0;
        break;
  }
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

//SPI Interrupt Routine
ISR (SPI_STC_vect)
{
  pinMode(MISO, OUTPUT);
  SPDR = array[i];
  i++;
  
  if(i > 3)
    i = 0;
}

