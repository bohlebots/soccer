#include "bohlebots.h"
BohleBots bot;
// Achtung: kein delay im Hauptprogramm nutzen !!!
// statt dessen immer bot.warte(ms)
// dauert genau so lange, überprüft aber den i2cBus in der Zeit,
// damit Taster und andere Sensoren weiterhin abgefragt werden.

int modus = 0;
int tastled=0;
void setup()
{
  bot.set_bot_type(3);
  bot.init();
  bot.set_pixy(true);
  bot.boardled(1, AUS);
  bot.boardled(2, AUS);

  for (int lauf=0; lauf<8 ; lauf++)
  {
    bot.led(tastled, 2, lauf);
    bot.led(tastled, 1, lauf);
    bot.warte(500);
  }
   
}

void loop()
{
  bot.warte(5); // muss immer in jede loop - Achtung: kein delay im
                // Hauptprogramm nutzen !!! während des "Wartens" werden per i2c
                // alle Sensoren ausgelesen Werte unter 2 machen keinen Sinn!!!

  if (bot.boardtast(4))
    {
      modus=1;
      
    }
    if (bot.boardtast(2))
    {
      modus=0;
      
    }
  else if (bot.boardtast(3))
    {
      bot.boardled(1, GRUEN);
      bot.fahre(0,0, 30);
    }
  else // nichts gedrückt
    {
      if (modus ==0)
      {
      bot.boardled(1, AUS);
      bot.boardled(2, AUS);
      bot.led(tastled, 1, AUS);
      bot.led(tastled, 2, AUS);
      if (bot.goalExists) bot.boardled(1, GRUEN);
      else bot.boardled(1, ROT);
      bot.fahre(0,0, 0);
      }
if (modus ==1)
      {
      bot.boardled(1, BLAU);
      bot.boardled(2, BLAU);
      bot.led(tastled, 2, BLAU);
      bot.led(tastled, 1, BLAU);
      bot.fahre(0,0, bot.ballDirection*15);
      }

    }

    schreibe();
}

void schreibe()
{
  Serial.println("Debug-Info");
  Serial.print("Ballrichtung : ");Serial.println(bot.ballDirection);
  Serial.print("Ballzone     : ");Serial.println(bot.ballDistance);
  Serial.print("Ball ist da  : ");Serial.println(bot.ballExists);
  Serial.print("kompass  : ");Serial.println(kompass_ena);
  Serial.print("Torrichtung : ");Serial.println(bot.goalDirection);
  Serial.print("Torentfernung : ");Serial.println(bot.goalDistance);
  Serial.println();
  Serial.println(bot.input(1));
  Serial.println();
  bot.taster(tastled, int nr)
}
