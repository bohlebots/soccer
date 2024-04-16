void schreibe()
{
  for (int lauf= 0; lauf < 16; lauf++)
  {
    //sprintf(s, "%4d", wert[lauf]);
    Serial.print (wert[lauf]); 
    Serial.print("\t");
  }                                           
  
  //Serial.println();Serial.println();
  Serial.print(" Ballrichtung(7 abziehen), Zone : ");
  
  if (ballmedian.getStatus()     == ballmedian.OK)     { ballmedian.getMedian(ballrichtung);   }
  if (balldistmedian.getStatus() == balldistmedian.OK) { balldistmedian.getMedian(zone);     }
  i2cdata=zone*16+ballrichtung;
  Serial.print(ballrichtung-7);       // as expected by master
  Serial.print(" , ");
  Serial.print(zone); 
  Serial.print("   i2cdata: ");
  Serial.println(i2cdata); 
  
  Serial.flush();
 

}


void tssp_init()
{
  for (int lauf= 0; lauf < 16; lauf++)
  {
    wert[lauf] = 0; 
  }
}  
void tssp_read() // liest alle 16 Sensorwerte aus und speicher sie im Puffer ab.
{
  zeit = 0;
  while (zeit < 5) //5ms sind genau 6 Durchläufe zu je 833Microsekunden
  for (int lauf = 0 ; lauf < 16 ; lauf++)
  {
    wert[lauf] += 1-digitalRead(tssp_pin[lauf]);                         
  }
}
void finde_max()
{
  wertmax=-1;
  zone          = 0;
  ballrichtung = 0;
  for (int lauf = 0; lauf < 16; lauf++)
  {
    if (wert[lauf] > wertmax)
    {
      wertmax = wert[lauf];
      ballrichtung=richtung[lauf]+7; // am Ende nur uint, daher +7 ... im Mainprogramm wieder -7   
    }
    if (wert[lauf] > 100) zone++;
  }
  ballmax       = wertmax;
  if (zone>15)zone =15;
  ballmedian.add(ballrichtung);
  balldistmedian.add(zone);
}

void messen() 
{
    tssp_init();
    tssp_read();     
    finde_max(); 
}
