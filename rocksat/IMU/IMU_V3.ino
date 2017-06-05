#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_HMC5883_U.h>

int ReadAxis(int);

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

const int xInput = A0;
const int yInput = A1;
const int zInput = A2; 

float xAccel;
float yAccel;
float zAccel;

float xGyro;
float yGyro;
float zGyro;

float xMag;
float yMag;
float zMag;
float headingDegrees;

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
  
}

void loop(void) 
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
