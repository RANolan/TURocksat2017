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

void displaySensorDetails(void)
{
  sensor_t gyroSensor;
  sensor_t magSensor;
  
  gyro.getSensor(&gyroSensor);
  mag.getSensor(&magSensor);
  
  Serial.println("------------------------------------");
  Serial.print  ("Gyro Sensor:  "); Serial.println(gyroSensor.name);
  Serial.print  ("Mag Sensor:   "); Serial.println(magSensor.name);
  Serial.print  ("Accel Sensor: "); Serial.println("ADXL377");
  delay(500);
}

void setup(void) 
{
  analogReference(EXTERNAL);
  Serial.begin(9600);
  Serial.println("IMU Data Retrieval Test: Gyro, Mag and Accel"); Serial.println("");
  
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
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
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

  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
  long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
  long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);
  
  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;

  /* Display the results (speed is measured in rad/s) */
  Serial.println("Accel Data");
  Serial.print("X: "); Serial.print(xAccel); Serial.print("G, ");
  Serial.print("Y: "); Serial.print(yAccel); Serial.print("G, ");
  Serial.print("Z: "); Serial.print(zAccel); Serial.println("G");
 
  /* Display the results (speed is measured in rad/s) */
  Serial.println("Gyro Data");
  Serial.print("X: "); Serial.print(gyroEvent.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(gyroEvent.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(gyroEvent.gyro.z); Serial.print("  ");
  Serial.println("rad/s ");

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.println("Mag Data");
  Serial.print("X: "); Serial.print(magEvent.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(magEvent.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(magEvent.magnetic.z); Serial.print("  ");Serial.println("uT");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(gyroEvent.magnetic.y, gyroEvent.magnetic.x);
  
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
  float headingDegrees = heading * 180/M_PI; 

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
