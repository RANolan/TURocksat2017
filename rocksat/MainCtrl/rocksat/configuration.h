#ifndef CONFIGURATION_H_   /* Include guard */
#define CONFIGURATION_H_


//SPI Bus
#define SPI_MISO 50
#define SPI_MOSI 51
#define SPI_SCLK 52

//I2C Bus - Pressure Sensor
#define PS_ADDR 0x28
#define PS_SCL 21
#define PS_SDA 20
#define PS_INT 36

//RS232
#define TX 14
#define RX 15

//Air Control
#define AC_SOLENOID 34
#define MINUTES_SOLENOID_OPEN 8

//MultiPurpose Chip Select
#define UV_CONV 24
#define TEVISO 3
#define UV_CLK 4
#define UV_DATA 48

//Gerdien Condenser
#define GC1_CS 30  //negative ions
#define GC2_CS 32 //positive ions
#define GC_RST 26
#define SECONDS_UNTIL_GC_RESET 4

//IMU
#define IMU_GSWITCH 28
#define IMU_TRIGGER 49

//SD Card
#define SD_CS 53


#endif
