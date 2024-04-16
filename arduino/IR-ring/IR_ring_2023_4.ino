// CoRoSoN IR-Ring 
// Version 2023_1 
// letzte Änderung : 10.10.2023
// Diese Datei muss nicht geändert werden !!!

#include <elapsedMillis.h>
#include <Wire.h>
#include "RunningMedian.h"
#include "header.h"      // eigentlich keine echter header, aber hier steht alles drin, was für alle Ringe gilt
#include "functions.h"   // alle Funktionen, die unverändert bleiben sollten
elapsedMillis testzeit;
void loop() 
{
  messen();
  delay(1);
  //Serial.println(testzeit);
  //check_i2c();
  //schreibe();     //nur diagnose
      
} // von loop 
