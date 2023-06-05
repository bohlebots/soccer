/*
 * Ver. 2023_05_08
 * rudimentäre Fahrlogik
 */ 

#include "BohleBots.h"
BohleBots bot;

#define LICHT_PIN 3

int tempo    = 40; // setzt das normale Fahrtempo
int maxTempo = 70; // setzt das maximal Fahrtempo

int altetorRichtung;
int alteRichtung;
int neueRichtung;
elapsedMillis richtungszeit;
// Achtung: kein delay im Hauptprogramm nutzen !!!
// statt dessen immer bot.warte(ms), mindestens 20ms
// dauert genau so lange, überprüft aber den i2cBus in der Zeit,
// damit Taster und andere Sensoren weiterhin abgefragt werden.

int modus = 0;

void setup() 
{
  bot = BohleBots(); 
  bot.linkesModul  = 1; //Zuordnung der TastsetLed Module
  bot.rechtesModul = 2; //auf die hardkodierten Adressen
                        // Auf den Modulen gilt: links=1, rechts=2  
  bot.setBotTyp(4);
  bot.setLichtPin(LICHT_PIN);
  bot.setBenutztPixy(true);
  bot.setBenutztKompass(false);
  bot.setBallSchwelle(500);
  bot.init();      
  
  Serial.println("kann losgehen...");
}


void changeModus()   //?
{
  if (bot.tastZeit < 1000) return;
  bot.tastZeit = 0;
  if (modus == 1) 
  {
    modus = 0;
    return;
  }
  if (modus == 0) 
  {
    modus = 1;
    bot.tastZeit = 500;
    return;
  }
}

void vorbereitung()
{
  bot.fahre(0, 0, 0);
  bot.setLed(bot.rechtesModul, 1, GRUEN);

  if (bot.taster(bot.rechtesModul, 2)) bot.kick(40);
  if (bot.taster(bot.linkesModul,  1)) bot.setKompassHeading();
  if (bot.taster(bot.linkesModul,  2)) bot.fahre(0, 0, 10);
  //if (bot.taster(bot.rechtesModul, 1)) //todo?

  if (!bot.siehtBall())             bot.setLed(bot.linkesModul, 2, ROT);
  else if (bot.ballRichtung() == 0) bot.setLed(bot.linkesModul, 2, GRUEN);
       else                         bot.setLed(bot.linkesModul, 2, MAGENTA);

  if (bot.hatBall()) bot.setLed(bot.rechtesModul, 2, BLAU); 
  else               bot.setLed(bot.rechtesModul, 2, AUS);

  //if      (abs(bot.kompass()) < 10)   bot.setLed(linkesModul, 1, GRUEN);      
  //else if (abs(bot.kompass()) < 30)   bot.setLed(linkesModul, 1, WEISS);
  //else if (abs(bot.kompass()) < 90)   bot.setLed(linkesModul, 1, MAGENTA);
  //else                                bot.setLed(linkesModul, 1, ROT);
  if (abs(bot.torRichtung()) < 60)  bot.setLed(bot.linkesModul, 1, BLAU);  
  if (abs(bot.torRichtung()) < 30)  bot.setLed(bot.linkesModul, 1, GELB);
  if (abs(bot.torRichtung()) < 10)  bot.setLed(bot.linkesModul, 1, GRUEN);
  if (!bot.siehtTor())              bot.setLed(bot.linkesModul, 1, ROT);
  //schreibe();
}

void strike()// Ball ist in der Ballschale
{
  bot.setLed(bot.rechtesModul, 2, BLAU); 
  if (bot.strikeZeit < 100) bot.fahre(0, maxTempo, bot.torRichtung() / 2);
  if (bot.strikeZeit < 5000) 
  {
    bot.fahre(0, 0, bot.torRichtung() / 2);
    bot.strikeZeit = 0;
    bot.kick(40);
  }
  else   //vielleicht raus???
  {
    bot.fahre(0, 0, 30);
  }
   
}

void suche() //wird ausgeführt, wenn der Ball an ist, aber nicht in der Ballschale ist
{  
    if (bot.ballRichtung() ==  0) neueRichtung =    0;
    if (bot.ballRichtung() ==  1) neueRichtung =   45;
    if (bot.ballRichtung() ==  2) neueRichtung =   90;
    if (bot.ballRichtung() ==  3) neueRichtung =   90;
    if (bot.ballRichtung() ==  4) neueRichtung =  135;
    if (bot.ballRichtung() ==  6) neueRichtung =  180;
    if (bot.ballRichtung() ==  5) neueRichtung =  135;
    if (bot.ballRichtung() ==  6) neueRichtung =  180;
    if (bot.ballRichtung() ==  7) neueRichtung = -135;
    if (bot.ballRichtung() ==  8) neueRichtung = -135;
    if (bot.ballRichtung() == -7) neueRichtung =  135;
    if (bot.ballRichtung() == -6) neueRichtung =  180;
    if (bot.ballRichtung() == -5) neueRichtung = -135;
    if (bot.ballRichtung() == -4) neueRichtung = -135;
    if (bot.ballRichtung() == -3) neueRichtung =  -90;
    if (bot.ballRichtung() == -2) neueRichtung =  -90;
    if (bot.ballRichtung() == -1) neueRichtung =  -45;

   bot.fahre(neueRichtung, tempo, bot.torRichtung() / 2);  
  if (alteRichtung != neueRichtung) bot.strikeZeit = 0;
    alteRichtung = neueRichtung;     
  
}
void play()
{
  if (bot.blindZeit < 2000)
  {
    bot.setLed(bot.rechtesModul, 1, WEISS);
    bot.setLed(bot.linkesModul,  2, AUS);
    if (!bot.siehtBall()) 
    {
      bot.setLed(bot.linkesModul, 2, ROT);
      bot.fahre(0, 0, bot.torRichtung() / 2);
    }
    else 
    {
      bot.setLed(bot.linkesModul, 2, AUS);
    
      if(bot.hatBall()) strike();
      else
      {
        bot.strikeZeit = 0;
        bot.setLed(bot.rechtesModul, 2, AUS);
        suche();
      }
    }

  }
  else 
  {
    //bot.fahre(0,0,30);
    bot.fahre(0, 0, altetorRichtung / 5);
  }
  if (bot.siehtTor()) 
  {
    bot.blindZeit = 0;
    altetorRichtung = bot.torRichtung();    
  }
}
/*
void play()
{
  bot.setLed(rechts,1,WEISS);
  bot.setLed(links,2,AUS);
  
  //bot.fahre(0,50,0);
  
  
}
*/
void loop()
{
  bot.warte(20); 
  
  
  if (bot.taster(bot.rechtesModul,1)) changeModus();
  if (modus == 0) vorbereitung();
  if (modus == 1) play();

}

void schreibe()
{ 
  Serial.println("+----------------+---------------------------------------");
  Serial.println("|  DEBUG START:  |                                       ");
  Serial.println("+----------------+                                       ");
  Serial.println("   [ Ball ]                                              ");
  Serial.println("        sieht:      " + String(bot.siehtBall()           ));
  Serial.println("        hat:        " + String(bot.hatBall()             ));
  Serial.println("        Richtung:   " + String(bot.ballRichtung()        ));
  Serial.println("        Entfernung: " + String(bot.ballEntfernung()      ));
  Serial.println("                                                         ");
  Serial.println("   [ Tor ]                                               ");
  Serial.println("        sieht:      " + String(bot.siehtTor()            ));
  Serial.println("        Richtung:   " + String(bot.torRichtung()         ));
  Serial.println("        Entfernung: " + String(bot.torEntfernung()       ));
  Serial.println("                                                         ");
  Serial.println("   [ Eigenes Tor ]                                       ");
  Serial.println("        sieht:      " + String(bot.siehtEigenesTor()     ));
  Serial.println("        Richtung:   " + String(bot.eigenesTorRichtung()  ));
  Serial.println("        Entfernung: " + String(bot.eigenesTorEntfernung()));
  Serial.println("                                                         ");
  Serial.println("   [ Lichtschranke ]                                     ");
  Serial.println("        Analogwert: " + String(bot.input(LICHT_PIN)      ));
  Serial.println("                                                         ");
  Serial.println("+---------------+                                        ");
  Serial.println("|  DEBUG ENDE:  |                                        ");
  Serial.println("+---------------+----------------------------------------");
}
