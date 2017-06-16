#include "configuration.h"
#include <Arduino.h>
#include <SD.h>


void createFile(char* filename){
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      break;  // leave the loop!
    }
  }


  
};

