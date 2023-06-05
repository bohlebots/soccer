// Pico ir_ring 
// Version 2022_1 i2cBus
// letzte Änderung : 05.12.2022
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
  //Serial.println("richtung: " + String(ballrichtung)); 
} // von loop
