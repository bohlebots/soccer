

// 27. November 2023
// Bohlebots
// header für BohleBots Hauptplatinen

// Einfach in das Verzeichnis mit der .ino Datei legen und benutzen
// die vier Motor-Outputs und die vier 12Bit Analog-Inputs sind vordefiniert
// motor(nr,speed) und input(nr)
// zusätzlich gibt es den Wrapper digit, der das Analoge Signal der Inputs als
// digital-Signal True oder False zurück gibt, True bei closed, False bei open

// Die Serielle Schnittstelle sowie der I2C Bus werden automatisch gestartet

// Interessant sind sicherlich noch  EEPROM, WIFI und BLE

/*
 * Festlegung der Farben der LEDs auf der tastled-Platine
 */
#define AUS 0
#define GRUEN 1
#define ROT 2
#define GELB 3
#define BLAU 4
#define CYAN 5
#define MAGENTA 6
#define WEISS 7

#define led1r 5
#define led1g 23
#define led1b 16

#define led2r 4
#define led2g 15
#define led2b 13

/*
 * Festlegung der Motoren im bot-type-Betrieb
 * type 2 : motor rechts 1, Motor links 2
 *
 * type 3 : motor rechts 1, Motor hinten 2, Motor links 3
 *
 * Fahre (richtung, geschw, drehung)
 * Richtung von -179 bis 180, 0 nach vorne - wird gequantelt
 * Geschw von -100 bis +100
 * dreh (von -100 bis + 100 , rechts rum positiv
 *
 */

// Achtung : kein delay im Hauptprogramm nutzen.
// statt dessen bohlebots.warte(ms)

#include <Pixy2I2C.h>
#include <Wire.h>
#include <elapsedMillis.h>
Pixy2I2C pixy;
#define SDA 21
#define SCL 22

#define DRIVE_DIS 27
#define DRIVE1_PWM 18
#define DRIVE1_DIR 19
#define DRIVE2_PWM 14
#define DRIVE2_DIR 12
#define DRIVE3_PWM 25
#define DRIVE3_DIR 26
#define DRIVE4_PWM 32
#define DRIVE4_DIR 33

#define INPUT1 35
#define INPUT2 34
#define INPUT3 39
#define INPUT4 36

//#define io1 17
//#define io2 2

#define kicker 17
#define dribbler 2
#define IR_ADDRESS 0x55
#define KOMPASS_ADRESSE 0x60 // cmps11, cmps12, cmps14
#define ANGLE_8 1
int kompass_wert;
int head;
bool kompass_ena = false;
bool hatPixy = false;

int bot_type = 3;

bool portena[] = {false, false, false, false, false, false, false, false};
int tastLedID[] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
bool taster1Array[] = {false, false, false, false, false, false, false, false};
bool taster2Array[] = {false, false, false, false, false, false, false, false};

int led1Array[] = {0, 0, 0, 0, 0, 0, 0, 0};
int led2Array[] = {0, 0, 0, 0, 0, 0, 0, 0};

elapsedMillis totzeit;
elapsedMillis wartezeit;
int irpaket;
class BohleBots
{
public:
  int ballDirection = 0;
  int ballDistance = 0;
  bool ballExists = false;
  int goalDirection = 0;
  int goalDistance = 0;
  bool goalExists = false;

  BohleBots()
  {
    Serial.begin(115200);
    Wire.begin(SDA, SCL);

    pinMode(DRIVE_DIS, OUTPUT);
    digitalWrite(DRIVE_DIS, LOW);
    pinMode(DRIVE1_DIR, OUTPUT);
    pinMode(DRIVE2_DIR, OUTPUT);
    pinMode(DRIVE3_DIR, OUTPUT);
    pinMode(DRIVE4_DIR, OUTPUT);

    pinMode(INPUT1, INPUT);
    pinMode(INPUT2, INPUT);
    pinMode(INPUT3, INPUT);
    pinMode(INPUT4, INPUT);

    pinMode(kicker, OUTPUT);  digitalWrite(kicker, LOW);
    pinMode(dribbler, OUTPUT);digitalWrite(dribbler, LOW);

    pinMode(led1r, OUTPUT);
    digitalWrite(led1r, HIGH);
    pinMode(led1g, OUTPUT);
    digitalWrite(led1g, HIGH);
    pinMode(led1b, OUTPUT);
    digitalWrite(led1b, HIGH);
    pinMode(led2r, OUTPUT);
    digitalWrite(led2r, HIGH);
    pinMode(led2g, OUTPUT);
    digitalWrite(led2g, HIGH);
    pinMode(led2b, OUTPUT);
    digitalWrite(led2b, HIGH);

    Wire.begin(SDA, SCL);

    ledcAttachPin(DRIVE1_PWM, 1);
    ledcSetup(1, 1000, 8);
    ledcWrite(1, 0);
    ledcAttachPin(DRIVE2_PWM, 2);
    ledcSetup(2, 1000, 8);
    ledcWrite(2, 0);
    ledcAttachPin(DRIVE3_PWM, 3);
    ledcSetup(3, 1000, 8);
    ledcWrite(3, 0);
    ledcAttachPin(DRIVE4_PWM, 4);
    ledcSetup(4, 1000, 8);
    ledcWrite(4, 0);
    boardled(1, AUS);
    boardled(2, AUS);
  }

  void init()
  {
    for (int lauf = 0; lauf < 8; lauf++)
      {
        Wire.beginTransmission(tastLedID[lauf]);
        byte error = Wire.endTransmission();
        if (error == 0)
          portena[lauf] = true;
        Serial.print("LED_Tast : " + String(lauf) + " : ");
        if (error == 0)
          Serial.println("true");
        else
          Serial.println("false");
      }
    delay(100);
    Wire.beginTransmission(KOMPASS_ADRESSE);
    byte error = Wire.endTransmission();
    if (error == 0)
      kompass_ena = true;
    if (error == 0)
      Serial.println("Kompass true");
    else
      Serial.println("Kompass false");

    if (hatPixy)
      {
        Serial.print("Warte auf Pixy2 auf i2c 0x54...");
        pixy.init(0x54);
        Serial.println("done");
      }
  }
  void warte(int zeit)
  {
    wartezeit = 0;
    i2csync();
    while (wartezeit < zeit)
      {
        if ((wartezeit % 10) == 0)
          i2csync();
        else
          delay(1);
      }
  }

  void motor(int number, int speed)
  {
    // Speed wird bei 100 und -100 gekappt
    if (speed > 100)
      speed = 100;
    if (speed < -100)
      speed = -100;
    int pwm = spdToPWM(speed);
    int dir;
    if (speed < 0)
      dir = LOW;
    else
      dir = HIGH;

    if (number == 1)
      digitalWrite(DRIVE1_DIR, dir);
    if (number == 2)
      digitalWrite(DRIVE2_DIR, dir);
    if (number == 3)
      digitalWrite(DRIVE3_DIR, dir);
    if (number == 4)
      digitalWrite(DRIVE4_DIR, dir);

    if ((number > 0) && (number < 5))
      ledcWrite(number, pwm);
  }

  void fahre(int richtung, int geschw, int dreh)
  {
    if (bot_type == 2)
      fahre2(geschw, dreh);
    if (bot_type == 3)
      fahre3(richtung, geschw, dreh);
  }

  void fahre2mot(int geschw, int dreh)
  {
    int maxs = abs(geschw) + abs(dreh);
    if (maxs > 100)
      {
        geschw = geschw * 100 / maxs;
        dreh = dreh * 100 / maxs;
      }
    motor(1, dreh + geschw);
    motor(2, dreh - geschw);
  }

  void set_bot_type(int t)
  {
    if (t < 2)
      t = 2;
    if (t > 3)
      t = 3;
  }
  void set_pixy(boolean t) { hatPixy = t; }
  void ena(bool isena) { digitalWrite(DRIVE_DIS, !isena); }

  int input(int number)
  {
    if (number == 1)
      return (analogRead(INPUT1));
    if (number == 2)
      return (analogRead(INPUT2));
    if (number == 3)
      return (analogRead(INPUT3));
    if (number == 4)
      return (analogRead(INPUT4));
  }

  int kompass() { return kompass_wert; }

  void setze_kompass()
  {
    Wire.beginTransmission(KOMPASS_ADRESSE);
    byte error = Wire.endTransmission();
    if (error == 0)
      {
        head = kompass_org();
      }
  }
  bool taster(int device,
              int nr) // liefert von device Taster nr (1:links, 2:rechts)
  {
    if (device < 0)
      return false;
    if (device > 7)
      return false;
    // portena[device] = true;
    if (nr == 1)
      return taster1Array[device];
    if (nr == 2)
      return taster2Array[device];
    return false;
  }

  void led(int device, int nr,
           int farbe) // setzt device led nr (1:links, 2:rechts) auf farbe

  {
    if (farbe < 0)
      return;
    if (farbe > 7)
      return;
    if (device < 0)
      return;
    if (device > 7)
      return;

    // portena[device] = true;

    if (nr == 1)
      {
        farbe = farbe * 2;
        led1Array[device] = farbe;
      }
    if (nr == 2)
      {
        farbe = farbe * 16;
        if (farbe > 63)
          farbe = farbe + 64;
        led2Array[device] = farbe;
      }
  }

  void boardled(int nr,
                int farbe) // setzt device led nr (1:links, 2:rechts) auf farbe

  {
    if (farbe < 0)
      return;
    if (farbe > 7)
      return;

    if (nr == 1)
      {
        digitalWrite(led1g, !(farbe & 1));
        digitalWrite(led1r, !(farbe & 2));
        digitalWrite(led1b, !(farbe & 4));
      }
    if (nr == 2)
      {
        digitalWrite(led2g, !(farbe & 1));
        digitalWrite(led2r, !(farbe & 2));
        digitalWrite(led2b, !(farbe & 4));
      }
  }

  bool boardtast(int nr) { return (!input(nr)); }

  void kick(int zeit)
  {
    if (totzeit < 1000)
      return;
    if (zeit > 40)
      zeit = 40;
    digitalWrite(kicker, HIGH);
    delay(zeit);
    digitalWrite(kicker, LOW);
    totzeit = 0;
  }

  bool digit(int number)
  {
    if (number == 1)
      return (analogRead(INPUT1) < 2048);
    if (number == 2)
      return (analogRead(INPUT2) < 2048);
    if (number == 3)
      return (analogRead(INPUT3) < 2048);
    if (number == 4)
      return (analogRead(INPUT4) < 2048);
  }

private:
  int spdToPWM(int speed)
  {
    if (speed < 0)
      speed *= -1;
    return ((speed * 255) / 100);
  }

  int kompass_org()
  {
    unsigned char high_byte, low_byte, angle8;
    unsigned int angle16;
    Wire.beginTransmission(KOMPASS_ADRESSE);
    Wire.write(ANGLE_8);
    Wire.endTransmission();
    Wire.requestFrom(KOMPASS_ADRESSE, 3);
    while (Wire.available() < 3)
      ;
    angle8 = Wire.read(); // Read back the 5 bytes
    high_byte = Wire.read();
    low_byte = Wire.read();
    angle16 = high_byte; // Calculate 16 bit angle
    angle16 <<= 8;
    angle16 += low_byte;
    return angle16 / 10;
  }

  int kompass_lesen()
  {
    return ((((kompass_org() - head) + 180 + 360) % 360) - 180);
  }

  void set_heading() { head = kompass_org(); }

  void i2csync()
  {
    for (int lauf = 0; lauf < 8; lauf++)
      {
        if (portena[lauf])
          {
            int ledwert = 255 - led1Array[lauf] - led2Array[lauf];
            Wire.beginTransmission(tastLedID[lauf]);
            Wire.write(ledwert);
            ;
            Wire.endTransmission();

            Wire.requestFrom(tastLedID[lauf], 1);
            if (Wire.available())
              {
                int tread = 255 - Wire.read();
                tread = tread % 128;
                if (tread > 63)
                  taster2Array[lauf] = true;
                else
                  taster2Array[lauf] = false;
                tread = tread % 2;
                if (tread > 0)
                  taster1Array[lauf] = true;
                else
                  taster1Array[lauf] = false;
              }
          }
      } // ENDE TastLED
    irpaket = 0;
    Wire.requestFrom(IR_ADDRESS, 1);
    if (Wire.available())
      {
        irpaket = Wire.read();
      }

    ballDirection = (irpaket % 16) - 7;
    ballDistance = irpaket / 16;
    ballExists = (ballDistance != 0);
    if (!ballExists)
      ballDirection = 0;

    if (kompass_ena == true)
      {
        kompass_wert = kompass_lesen();
      }
    if (hatPixy)
      {
        pixy_read();
      }
  }

  void
  pixy_auswerten() // wird nur aufgerufen, wenn die Pixy überhaupt etwas sieht
  {
    int my_signature = 1; // wir spielen immer auf Signatur 1
    int sieht_farbe = pixy.ccc.blocks[0].m_signature;
    if (sieht_farbe == my_signature)
      {
        goalDirection = (pixy.ccc.blocks[0].m_x - 158) / 2;
        int tor_breite = pixy.ccc.blocks[0].m_width;
        int tor_hoehe = pixy.ccc.blocks[0].m_height;
        // tor_entfernung_roh =  pixy.ccc.blocks[0].m_y-80;
        int tor_entfernung_roh = pixy.ccc.blocks[0].m_y;
        goalDistance =
            (tor_entfernung_roh - tor_hoehe) / 4; //-abs(tor_richtung)/10;
        if (goalDistance < 0)
          goalDistance = 0;
        if (goalDistance > 63)
          goalDistance = 63;
      }
  }

  void pixy_read()
  {
    int i;
    // grab blocks!
    pixy.ccc.getBlocks();
    goalExists=false;
    // If there are detect blocks, print them!
    if (pixy.ccc.numBlocks)
      {
        pixy_auswerten();
        goalExists=true;
      }
    else
      {
        goalDirection = kompass();
      }
  }

  void fahre2(int geschw, int dreh)
  {
    int maxs = abs(geschw) + abs(dreh);
    if (maxs > 100)
      {
        geschw = geschw * 100 / maxs;
        dreh = dreh * 100 / maxs;
      }
    motor(1, dreh + geschw);
    motor(2, dreh - geschw);
  }

  void fahre3(int richtung, int geschw, int dreh)
  {
    richtung = richtung / 60;
    int maxs = abs(geschw) + abs(dreh);
    if (maxs > 100)
      {
        geschw = geschw * 100 / maxs;
        dreh = dreh * 100 / maxs;
      }
    if (richtung == 0) // geradeaus
      {
        motor(1, -geschw + dreh);
        motor(2, +dreh);
        motor(3, geschw + dreh);
      }

    if (richtung == 1) // 60 Grad rechts
      {
        motor(1, +dreh);
        motor(2, geschw + dreh);
        motor(3, -geschw + dreh);
      }

    if (richtung == -1) // -60 Grad links
      {
        motor(1, -geschw + dreh);
        motor(2, geschw + dreh);
        motor(3, +dreh);
      }

    if (richtung == 3) // zurück
      {
        motor(1, geschw + dreh);
        motor(2, +dreh);
        motor(3, -geschw + dreh);
      }

    if (richtung == -2) // -120 Grad links
      {
        motor(1, +dreh);
        motor(2, -geschw + dreh);
        motor(3, geschw + dreh);
      }

    if (richtung == 2) // 120 Grad rechts
      {
        motor(1, geschw + dreh);
        motor(2, -geschw + dreh);
        motor(3, +dreh);
      }
  }
};
