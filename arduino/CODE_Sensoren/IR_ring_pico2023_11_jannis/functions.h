void schreibe()
{
  /*for (int lauf= 0; lauf < 15; lauf++)
  {
    //sprintf(s, "%4d", wert[lauf]);
    Serial.print (wert[lauf]); 
    Serial.print("\t");
  } */                                          
  /*
  Serial.println();Serial.println();
  Serial.println(" Ballrichtung, Zone");
  
  if (ballmedian.getStatus()     == ballmedian.OK)     { ballmedian.getMedian(ballrichtung);   }
  if (balldistmedian.getStatus() == balldistmedian.OK) { balldistmedian.getMedian(zone);     }
  i2cdata=zone*100+ballrichtung;
  Serial.print(ballrichtung);       // as expected by master
  Serial.print(" , ");
  Serial.print(zone); 
  Serial.print("   i2cdata: ");
  Serial.println(i2cdata); 
  
  Serial.flush();
  */

  //Serial.println(wert[0]);

  for(int i = 0; i < 15; i++) {
    Serial.print(" " + String(wert[i]) + ",");
  }
  Serial.println();
}


void tssp_init()
{
  for (int lauf= 0; lauf < 15; lauf++)
  {
    wert[lauf] = 0; 
  }
}  
void tssp_read() // liest alle 15 Sensorwerte aus und speicher sie im Puffer ab.
{
  zeit = 0;
  while (zeit < 5) //5ms sind genau 6 Durchläufe zu je 833Microsekunden
  for (int lauf = 0 ; lauf < 15 ; lauf++)
  {
    wert[lauf] += 1-digitalRead(tssp_pin[lauf]);                         
  }
}
void finde_max()
{
  wertmax=-1;
  ballrichtung = -7;
  for (int lauf = 0; lauf < 15; lauf++)
  {
    if(wert[lauf] == 0) { // 5% bonus für sensoren mit richtung 0 um zucken zu verhindern
      wert[lauf] = (wert[lauf] * 105) / 100;
    }
    if (wert[lauf] > wertmax)
    {
      wertmax      = wert[lauf];
      ballrichtung = richtung[lauf]; // am Ende nur uint, daher +7 ... im Mainprogramm wieder -7   
    }
  }
  ballmax       = wertmax;
  ballmedian.add(ballrichtung);
}

int calc_distanz() {
  static int maxSum = 1;
  int sum = 0;
  for(int i = 0; i < 15; i++) {
    sum += wert[i] / distanz_teiler[i];
  }       
  if(sum > maxSum) {
    maxSum = sum;
  }
  int result = 15 - ((sum * 15) / maxSum);                                                                                                                                                                                                                         
  return result;
}

void messen() 
{
    tssp_init();
    tssp_read();     
    finde_max(); 
    distanz = calc_distanz();
    if(ballmax < 150) distanz = 0;
}
