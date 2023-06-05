// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/**
* ! BOHLEBOTS EEPROM HEADER
*  
* ? Version: 0.1.0
* ? Datum: 16. Mai 2023
*
* * Dieser Header bietet eine abstrahierte Schnittstelle zum EEPROM.
*
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
#ifndef BB_EEPROM_H
#define BB_EEPROM_H
//////////////////////////////////////////////////////////////////////
/*
*   INCLUDES / DEPENDENCIES
*/
//////////////////////////////////////////////////////////////////////
//
// Sollte hier ein Compiler-Fehler auftreten, muss die entsprechende
// Bilbliothek installiert werden.
//
#include <EEPROM.h>
//////////////////////////////////////////////////////////////////////
/*
*   DEFINES
*/
//////////////////////////////////////////////////////////////////////
//
// Adresse im eeprom an die geschrieben und von der gelesen wird
//
#define BB_EEPROM_RW_ADD 0
//////////////////////////////////////////////////////////////////////
/*
*   DEKLARATIONEN
*/
//////////////////////////////////////////////////////////////////////
//
// Struct um alle Daten, die im eeprom gesichert werden sollen zu sammeln
//
struct BB_EEPROM_DATA {
  bool IsInStandby;
  // hier erweitern wenn mehr gespeichert werden soll
};
/************************************************************
*
* ! [SETUP]
* ? BB_EEPROM_INIT()
*   
* * Beschreibung:
*     Initialisiert das EEPROM
*     Rueckgabewert:
*       0 => OK, 
*     < 0 => FAIL
*
************************************************************/
int BB_EEPROM_INIT();
/************************************************************
*
* ? BB_EEPROM_READ()
*   
* * Beschreibung:
*     liest die aktuellen Werte aus dem EEPROM und 
*     gibt diese zurueck
*
************************************************************/
BB_EEPROM_DATA BB_EEPROM_READ();
/************************************************************
*
* ? BB_EEPROM_READ()
*   
* * Beschreibung:
*     Schreibt die uebergebenen Daten in das EEPROM
*     Rueckgabewert:
*       0 => OK, 
*     < 0 => FAIL
*
************************************************************/
int BB_EEPROM_WRITE(const BB_EEPROM_DATA& data);
//
// END OF FILE
//
#endif  // BB_EEPROM_H