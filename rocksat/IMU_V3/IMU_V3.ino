
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

FLOATUNION_t xAccel;
FLOATUNION_t yAccel;
FLOATUNION_t zAccel;

FLOATUNION_t xGyro;
FLOATUNION_t yGyro;
FLOATUNION_t zGyro;

FLOATUNION_t xMag;
FLOATUNION_t yMag;
FLOATUNION_t zMag;
FLOATUNION_t headingDegrees;

// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = 509;
int xRawMax = 512;
 
int yRawMin = 506;
int yRawMax = 514;
 
int zRawMin = 508;
int zRawMax = 520;
 
void setup(void) 
{
  analogReference(EXTERNAL);
  attachInterrupt(interruptPin, readData, RISING);
  
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

   // turn on interrupts
  SPCR |= _BV(SPIE);

  Serial.begin(9600);
  
 /*
  Serial.begin(9600);
 
  //Initialise the sensor 
  
  if(!gyro.begin())
  {
  
    //There was a problem detecting the L3GD20 ... check your connections 
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  
  //Initialise the sensor 
  if(!mag.begin())
  {
    //There was a problem detecting the HMC5883 ... check your connections 
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  */
}

void loop(void) 
{
  /*does nothing*/
}

void readData()
{
  /* Get a new sensor event */ 
  sensors_event_t gyroEvent; 
  sensors_event_t magEvent;
  
  gyro.getEvent(&gyroEvent);
  mag.getEvent(&magEvent);

  int xRaw = ReadAxis(xInput);
  int yRaw = ReadAxis(yInput);
  int zRaw = ReadAxis(zInput);
  
  // re-scale to fractional Gs
  xAccel.number = (map(xRaw, xRawMin, xRawMax, -1000, 1000)) / 1000.0;
  yAccel.number = (map(yRaw, yRawMin, yRawMax, -1000, 1000)) / 1000.0;
  zAccel.number = (map(zRaw, zRawMin, zRawMax, -1000, 1000)) / 1000.0;
 
  /* Display the results (speed is measured in rad/s) */
  xGyro.number = gyroEvent.gyro.x;
  yGyro.number = gyroEvent.gyro.y;
  zGyro.number = gyroEvent.gyro.z;

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  xMag.number = magEvent.magnetic.x;
  yMag.number = magEvent.magnetic.y;
  zMag.number = magEvent.magnetic.z;

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(yMag.number, xMag.number);
  
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
  headingDegrees.number = heading * 180/M_PI; 

  //Serial.print("Heading (degrees): "); Serial.println(headingDegrees.number);

  Serial.print("Accel: ");
  Serial.print("X: "); Serial.print(xAccel.number); Serial.print("  ");
  Serial.print("Y: "); Serial.print(yAccel.number); Serial.print("  ");
  Serial.print("Z: "); Serial.print(zAccel.number); Serial.println("  ");

  Serial.print("Gyro: ");
  Serial.print("X: "); Serial.print(xGyro.number); Serial.print("  ");
  Serial.print("Y: "); Serial.print(yGyro.number); Serial.print("  ");
  Serial.print("Z: "); Serial.print(zGyro.number); Serial.println("  ");

  Serial.print("Mag: ");
  Serial.print("X: "); Serial.print(xMag.number); Serial.print("  ");
  Serial.print("Y: "); Serial.print(yMag.number); Serial.print("  ");
  Serial.print("Z: "); Serial.print(zMag.number); Serial.print("  ");
  Serial.print("Heading: "); Serial.println(headingDegrees.number);
  
  //delay(500);
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

//SPI Interrupt Routine
ISR (SPI_STC_vect)
{
  SPI.transfer('X');
  xAccel.number = 2.56;
  
  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(xAccel.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(yAccel.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(yAccel.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(xGyro.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(yGyro.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(zGyro.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(xMag.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(yMag.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(zMag.b[i]);
  }

  for(int i = 3; i >= 0; i--)
  {
    SPI.transfer(headingDegrees.b[i]);
  }

}

