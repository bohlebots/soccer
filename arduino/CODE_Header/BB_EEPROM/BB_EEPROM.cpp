// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/**
*      BOHLEBOTS EEPROM CPP-FILE
*     
*      Version: 0.1.0
*      Date: 16. Mai 2023
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------

#include "BB_EEPROM.h"

//////////////////////////////////////////////////////////////////////
/*
*   IMPLEMENTIERUNGEN
*/
//////////////////////////////////////////////////////////////////////

int BB_EEPROM_INIT() {
  if (!EEPROM.begin(sizeof(BB_EEPROM_DATA))) {
    return -1;
  }
  return 0;
}



BB_EEPROM_DATA BB_EEPROM_READ() {
  BB_EEPROM_DATA data;
  memset(&data, 0, sizeof(BB_EEPROM_DATA)); // Init data with 0s
  EEPROM.readBytes(BB_EEPROM_RW_ADD, &data, sizeof(BB_EEPROM_DATA));
  return data;
}



int BB_EEPROM_WRITE(const BB_EEPROM_DATA& data) {
  EEPROM.writeBytes(BB_EEPROM_RW_ADD, &data, sizeof(BB_EEPROM_DATA));
  if (!EEPROM.commit()) {
    return -1;
  }
  return 0;
}