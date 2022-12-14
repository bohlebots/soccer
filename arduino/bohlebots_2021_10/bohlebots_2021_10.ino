int tempo = 30;  // setzt das maximal Fahrtempo
/*
 * Ver. 2021_07
 * rudimentäre Fahrlogik
 */
#include "bohlebots.h"
BohleBots bot;
// Achtung: kein delay im Hauptprogramm nutzen !!!
// statt dessen immer bot.warte(ms), mindestens 20ms
// dauert genau so lange, überprüft aber den i2cBus in der Zeit,
// damit Taster und andere Sensoren weiterhin abgefragt werden.

                                                                                                                                                                                                                                                                                                                                                                                               void setup() 
{
  links=3;      //Zuordnung der TastLED Module
  rechts=1;     //auf die hardkodierten Adressen
  // Auf den Modulen gilt: recht=1, links=2  
  
  bot.set_bot_type(4);
  bot.set_soccer(true);
  bot.set_pixy(true);
  bot.set_ball_schwelle(500);
  bot.init();      
  

  Serial.println("kann losgehen...");
}


void changeModus()
{
  if (tastzeit < 1000) return;
  tastzeit = 0;
  
  if (modus == 1) 
  {
    modus=0;
    return;
  }
  
  if (modus == 0) 
  {
    modus=1;
    tastzeit=500;
    return;
  }
}

void vorbereitung()
{
  bot.fahre(0,0,0);
  bot.led(rechts,1,GRUEN);

  if (bot.taster(rechts,2)) bot.kick(40);
  if (bot.taster(links,2)) bot.setze_kompass();
  if (bot.taster(links,1)) bot.fahre(0,0,30);
  //if (bot.taster(links,1)) //todo?

  if (bot.ball()==false) bot.led(links,1,ROT);
  else  if (bot.ballrichtung()==0) bot.led(links,1,GRUEN);
        else bot.led(links,1,BLAU);

  if      (abs(bot.torrichtung())<10)   bot.led(links,2,GRUEN);      
  else if (abs(bot.torrichtung())<30)   bot.led(links,2,GELB);
  else if (abs(bot.torrichtung())<90)   bot.led(links,2,BLAU);
  else                                  bot.led(links,2,ROT);
  if (hat_ball) bot.led(rechts,2,BLAU); else bot.led(rechts,2,AUS);

  //schreibe();
}

void strike()// Ball ist in der Ballschale
{
  bot.led(rechts,2,BLAU); 
  if (striketime<200) bot.fahre(0,90,bot.torrichtung()/2);
  else 
  {
    bot.kick(40);
    bot.fahre(0,0,bot.torrichtung()/2);
    striketime=0;
  }
  
}

void suche() //wird ausgeführt, wenn der Ball an ist, aber nicht in der Ballschale ist
{
  bot.fahre(0,0,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  0) bot.fahre (   0,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  1) bot.fahre (  45,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  2) bot.fahre (  90,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  3) bot.fahre (  90,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  4) bot.fahre ( 135,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  5) bot.fahre ( 180,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  6) bot.fahre ( 180,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  7) bot.fahre (-135,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() ==  8) bot.fahre ( -90,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() == -1) bot.fahre ( -45,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() == -2) bot.fahre ( -90,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() == -3) bot.fahre ( -90,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() == -4) bot.fahre ( 135,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() == -5) bot.fahre ( 180,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() == -6) bot.fahre ( 135,tempo,bot.torrichtung()/2);
  if (bot.ballrichtung() == -7) bot.fahre (  90,tempo,bot.torrichtung()/2);
  
  
}

void play()
{
  bot.led(rechts,1,WEISS);
  bot.led(links,2,AUS);
  if (bot.ball()==false) 
  {
    bot.led(links,1,ROT);
    bot.fahre(0,0,bot.torrichtung()/2);
  }
  else 
  {
    bot.led(links,1,AUS);
  
    if(hat_ball) strike();
    else
    {
      striketime=0;
      bot.led(rechts,2,AUS);
      suche();
    }
  }
}

void loop()
{
  bot.warte(20); 
  
  
  if (bot.taster(rechts,1)) changeModus();
  if (modus == 0) vorbereitung();
  if (modus == 1) play();

//Serial.println(hat_ball); // fast 20ms pro Durchlauf
}

void schreibe()
{ 
  Serial.println("Ball (sieht, hat, Richtung): " + String(bot.ball())+",   "+String(hat_ball)+",   "+String(bot.ballrichtung()));
  Serial.println("Torrichtung                : " + String(bot.torrichtung()));
}
