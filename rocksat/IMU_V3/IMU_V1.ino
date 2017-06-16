#include <Wire.h>
#include <EEPROM.h>
#include "SPI.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);

int addr = 0;
float gyro_x_val;
float gyro_y_val;
float gyro_z_val;
int SS = 10;
int MOSI = 11;
int MISO = 12;
int SCLK = 13;

void displaySensorDetails(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  //Serial.println("Gyroscope Test"); Serial.println("");
  
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }

  pinMode(SCLK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  gyro.getEvent(&event);

  gyro_x_val = event.gyro.x;
  gyro_y_val = event.gyro.y;
  gyro_z_val = event.gyro.z;

  EEPROM.write(addr, gyro_x_val);
  addr = addr + 1;
  if(addr ==EEPROM.length())
    addr = 0;
  EEPROM.write(addr, gyro_y_val);
  addr = addr + 1;
  if(addr ==EEPROM.length())
    addr = 0;
  EEPROM.write(addr, gyro_z_val);
  addr = addr + 1;
  if(addr ==EEPROM.length())
    addr = 0;

  printGyroData(gyro_x_val, gyro_y_val, gyro_z_val);
  delay(500);
}

void printGyroData(float gyroX, float gyroY, float gyroZ)
{
  /* Display the results (speed is measured in rad/s) */
  Serial.print("X: "); Serial.print(gyroX); Serial.print("  ");
  Serial.print("Y: "); Serial.print(gyroY); Serial.print("  ");
  Serial.print("Z: "); Serial.print(gyroZ); Serial.print("  ");
  Serial.println("rad/s ");
}

