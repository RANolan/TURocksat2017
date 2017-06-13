#include <SPI.h>
#include <Wire.h>

typedef union
{
  float number;
  uint8_t b[4];
}FLOATUNION_t;

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

void setup() {
  Serial.begin (115200);
  Serial.println ("Start");
  
  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop() 
{

  byte junkByte;
  digitalWrite(40, HIGH);
  delay(250);
  digitalWrite(40, LOW);
  delay(1000);
  digitalWrite(SS, LOW);

  junkByte = SPDR;

  //xAccel
  for(int i = 3; i >= 0; i--)
  {
    xAccel.b[i] = SPDR;
  }

  //yAccel
  for(int i = 3; i >= 0; i--)
  {
    yAccel.b[i] = SPDR;
  }

  //zAccel
  for(int i = 3; i >= 0; i--)
  {
    zAccel.b[i] = SPDR;
  }

  //xGyro
  for(int i = 3; i >= 0; i--)
  {
    xGyro.b[i] = SPDR;
  }

  //yGyro
  for(int i = 3; i >= 0; i--)
  {
    yGyro.b[i] = SPDR;
  }

  //zGyro
  for(int i = 3; i >= 0; i--)
  {
    zGyro.b[i] = SPDR;
  }

  //xMag
  for(int i = 3; i >= 0; i--)
  {
    xMag.b[i] = SPDR;
  }

  //yMag
  for(int i = 3; i >= 0; i--)
  {
    yMag.b[i] = SPDR;
  }

  //zMag
  for(int i = 3; i >= 0; i--)
  {
    zMag.b[i] = SPDR;
  }

  //headingDegrees
  for(int i = 3; i >= 0; i--)
  {
    headingDegrees.b[i] = SPDR;
  }

  
  digitalWrite(SS, HIGH);

  Serial.println("IMU Read");
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

  delay(1000);
}

