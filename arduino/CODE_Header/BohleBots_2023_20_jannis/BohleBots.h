// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/**
* ! BOHLEBOTS HEADER
*  
* ? Version: 0.1.3
* ? Datum: 8. Mai 2023
*
* * Dieser Header ist eine Sammlung und Kapselung der 
* * Standard-Funktionalitäten eines RoboCup-Soccer-Bots.
* * Die hier verwendeten Funktionen und Variablen sind aufgearbeitete 
* * Erfahrungswerte, die die BohleBots der jahrelang im RoboCup Soccer
* * sammeln konnten.
*
* ! Informationen zu den Robotern BohleBots in den letzten Jahren:
* ?    https://www.bohlebots.de/statisch/doku.html
* ?    https://github.com/stiebel/bohlebots
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
#ifndef BOHLEBOTS_H
#define BOHLEBOTS_H
//////////////////////////////////////////////////////////////////////
/*
*   INCLUDES / DEPENDENCIES
*/
//////////////////////////////////////////////////////////////////////
//
// Sollte hier ein Compiler-Fehler auftreten, muss die entsprechende 
// Bilbliothek installiert werden.
//
#include <Wire.h>
#include <elapsedMillis.h>
#include <Pixy2I2C.h>
//////////////////////////////////////////////////////////////////////
/*
*   DEFINES
*/
//////////////////////////////////////////////////////////////////////
//
// DEBUG
//
#define BOHLEBOTS_DEBUG // zeile auskommentieren um delays, statusblinks, etc. zu deaktivieren
//
// I2C Bus
//
#define SDA 21
#define SCL 22
//
// I2C Device-Adressen
//
#define PIXY_ADRESSE    0x54
#define IR_ADRESSE      0x55
#define KOMPASS_ADRESSE 0x60
#define ANGLE_8            1
// 
// Motor PORTS
// 
#define DRIVE_DIS   2
#define DRIVE1_PWM 12
#define DRIVE1_DIR 13
#define DRIVE2_PWM 27
#define DRIVE2_DIR 14
#define DRIVE3_PWM 26
#define DRIVE3_DIR 25
#define DRIVE4_PWM 19
#define DRIVE4_DIR 18
// 
// Analog PINs
// 
#define INPUT1 35
#define INPUT2 34
#define INPUT3 39
#define INPUT4 36
//
// Digitale PORTS
//
#define kicker   15
#define dribbler 23
//////////////////////////////////////////////////////////////////////
/*
*   DEKLARATIONEN
*/
//////////////////////////////////////////////////////////////////////
//
// Zuweisung: Farbwert - Name / Numerierung der Farben
//
enum FARBE {
  AUS = 0,
  GRUEN,
  ROT,
  GELB,
  BLAU,
  CYAN,
  MAGENTA,
  WEISS
};
//
// BohleBots Klasse
//
class BohleBots {
  public:
    /*
    * Alles ab hier ist public 
    * also von ausserhalb dieser Klasse erreichbar
    */
    //
    // elapsedMillis zur FREIEN VERWENDUNG!
    //
    // Idee: Kurz Schub aufzunehmen, bevor gekickt wird
    elapsedMillis strikeZeit;
    // Idee: Tracken wie lange das Tor nicht gesehen wurde 
    //       und parallel immer die letzte torRichtung speichern
    elapsedMillis blindZeit;  
    // Idee: Cool-Down-Timer fuer die Taster-Auslesung, 
    //       da loop viel schneller als menschlisches Druecken
    elapsedMillis  tastZeit;  
    //
    // Jeder Roboter besitzt auf dem InfraRot-Ring 2 steckbare Module
    // Diese muessen durch festes verloeten eine Adresse bekommen, 
    // welche in diese beiden Werte gesetzt werden muessen
    //
    int linkesModul  = 0;
    int rechtesModul = 0;
    //////////////////////////////////////
    /*
    *   SETUP-FUNKTIONEN
    */
    //////////////////////////////////////
    /*
    * Diese Funktionen sollten bei Verwendung
    * dieses Headers einmalig am Anfang des Programmes in einer 
    * bestimmten Reihenfolge aufgerufen werden!
    */
    /************************************************************
    *
    * ! [1. Aufruf]
    * ? BohleBots()
    *   
    * * Beschreibung:
    *     der Konstruktor erzeugt ein neues Objekt 
    *     (bzw. eine neue Instanz) der BohleBots Klasse
    *
    ************************************************************/    
    BohleBots();
    /************************************************************
    *
    * ! [2. - 5. Aufruf]
    * ? BohleBots::setBotTyp(int)
    *   
    * *  Beschreibung:
    *     setzt die Anzahl der anzutreibenen Raeder
    *
    * *  Parameter:
    *     anzahl: Anzahl der Raeder die angetrieben 
    *             werden muessen um zu fahren
    *
    *  ! Achtung:
    *     Dieser Header akzeptiert nur 2, 3 oder 4 Raeder!
    *     anzahl < 2 => intern: anzahl = 2 
    *     anzahl > 4 => intern: anzahl = 4
    *
    ************************************************************/  
    void setBotTyp(int anzahl);
    /************************************************************
    *
    * ! [2. - 5. Aufruf]
    * ? BohleBots::setLichtPin(int)
    *   
    *  * Beschreibung:
    *     setzt an welcher der 4 analogen Inputs die Lichtschranke 
    *     angeschlossen ist
    *
    *  * Parameter:
    *     nummer: 1 => INPUT1, 
    *             2 => INPUT2, 
    *             3 => INPUT3, 
    *             4 => INPUT4
    *
    *  ! Achtung:
    *     nummer < 1 oder nummer > 4 
    *       => hatBall() gibt immer false zurueck
    *
    ************************************************************/  
    void setLichtPin(int nummer);
    /************************************************************
    *
    * ! [2. - 5. Aufruf]
    * ? BohleBots::setBallSchwelle(unsigned int)
    *   
    *  * Beschreibung:
    *     setzt die Schwelle des analogen Wertes 
    *     fuer die Lichtschranke
    *
    *  * Parameter:
    *     schwelle: Analogwert der als Schwelle 
    *               gesetzt werden soll
    *
    *  ! Achtung:
    *     Lichtschrankenwert > Schwelle => Ball in Schale
    *     Lichtschrankenwert < Schwelle => Ball nicht in Schale
    *
    ************************************************************/
    void setBallSchwelle(unsigned int schwelle);
    /************************************************************
    *
    * ! [2. - 5. Aufruf]
    * ? BohleBots::setBenutztPixy(bool)
    *   
    *   * Beschreibung:
    *     setzt ob eine Pixy2 verwendet wird oder nicht
    *
    *  * Parameter:
    *     @benutzt: true / 1, wenn eine Pixy benutzt wird
    *               false / 0 wenn nicht
    *
    *  ! Achtung:
    *     Falls falsch gesetzt haengt sich der I2C Bus auf 
    *     und der Roboter tut nichts!
    *
    ************************************************************/
    void setBenutztPixy(bool benutzt);
    /************************************************************
    *
    * ! [2. - 5. Aufruf]
    * ? BohleBots::setBenutztKompass(bool)
    *   
    *  * Beschreibung:
    *     setzt ob eine Kompasssensor (CMPS11, CMPS12 oder CMPS14) 
    *     verwendet wird oder nicht
    *
    *  * Parameter:
    *     benutzt: true  / 1, wenn einen Kompass benutzt wird
    *              false / 0 wenn nicht
    *
    *  ! Achtung:
    *     Falls falsch gesetzt haengt sich der I2C Bus auf 
    *     und der Roboter tut nichts!
    *
    ************************************************************/
    void setBenutztKompass(bool benutzt);
    /************************************************************
    *
    * ! [6. Aufruf]
    * ? BohleBots::init()
    *   
    *  * Beschreibung:
    *     initialisiert des Roboter-Objekt, 
    *     bereitet also alle PINs, PORTs, und Verbindungen vor
    *
    ************************************************************/ 
    void init();    
    //////////////////////////////////////
    /*
    *   LOOP-FUNKTIONEN
    */
    //////////////////////////////////////
    //
    // Diese Funktionen koennen jederzeit aufgerufen werden!
    //
    /************************************************************
    *
    * ! [!MUST HAVE IN JEDEM LOOP-DURCHLAUF!]
    * ? BohleBots::warte(int)
    *   
    *  * Beschreibung:
    *     laesst den Roboter eine gewisse Zeit warten
    *
    *  * Paramter:
    *     zeit: Anzahl der Millisekunden, 
    *           die der Roboter warten soll
    *
    *  ! Achtung:
    *     Im Gegensatz zu einem delay legt diese Funktion
    *     nicht den gesamten Roboter lahm!
    *     Alle 10 ms werden die aktuellen Sensordaten gelesen!
    *
    ************************************************************/ 
    void warte(int zeit);
    /************************************************************
    *
    * ? BohleBots::fahre(int, int, int)
    *   
    *  * Beschreibung:
    *     laesst den Roboter in eine bestimmte Richtung,
    *     mit einem bestimmten Tempo fahren und sich dabei 
    *     gleichzeitig um die eigene Achse drehen
    *
    *  * Paramter:
    *     richtung: Richtung in die gefahren werden soll
    *               BotTyp:       3         |       4
    *               ------------------------+-----------------
    *                  -3 :  xxxxxxxxxxx    |  135° links
    *                  -2 :   120° links    |   90° links
    *                  -1 :    60° links    |   45° links
    *                   0 :    0° / vorne   |    0° / vorne
    *                   1 :    60° rechts   |   45° rechts
    *                   2 :   120° rechts   |   90° rechts
    *                   3 :   180° / hinten |  135° rechts
    *                   4 :  xxxxxxxxxxx    |  180° / hinten
    *     tempo:    Geschwindigkeit des Roboters in Prozent
    *     drehung:  Drehgeschwingikeit des Roboters in Prozent
    *
    *   ! Achtung:
    *       tempo + drehung kleiner als 100 halten!
    *       Wenn richtung einen Wert besitzt der nicht 
    *       in der Tabelle steht, wird kein neuer Befehl 
    *       an die Motoren gesendet!
    *       Eine Implementierung mit BotTyp 2 existiert nicht!
    *
    ************************************************************/ 
    void fahre(int richtung, int tempo, int drehung);
    /************************************************************
    *
    * ? BohleBots::siehtBall()
    *   
    *  * Beschreibung:
    *      gibt zurueck ob der InfraRot-Ring den Ball sieht 
    *
    ************************************************************/
    bool siehtBall();
    /************************************************************
    *
    * ? BohleBots::ballRichtung()
    *   
    *  * Beschreibung:
    *      gibt die BallRichtung vom InfraRot-Ring zurueck
    *      hierbei gilt:
    *       -7 => 157,5° links
    *       -6 => 135°   links
    *       -5 => 115,5° links
    *       -4 =>  90°   links
    *       -3 =>  67,5° links
    *       -2 =>  45°   links
    *       -1 =>  22,5° links
    *        0 =>   0° / vorne
    *        1 =>  22,5° rechts
    *        2 =>  45°   rechts
    *        3 =>  67,5° rechts
    *        4 =>  90°   rechts
    *        5 => 115,5° rechts
    *        6 => 135°   rechts
    *        7 => 157,5° rechts
    *        8 => 180° / hinten
    *
    *  ! Achtung:
    *     Wenn kein Ball gesehen wird, 
    *     ist der Rueckgabewert -7!
    *
    ************************************************************/
    int ballRichtung();
    /************************************************************
    *
    * ? BohleBots::hatBall()
    *   
    *  * Beschreibung:
    *      gibt zurueck ob der Ball in der Lichtschranke ist,
    *      indem der Analogwert der Lichtschranke mit der 
    *      BallSchwelle verglichen wird
    *
    ************************************************************/
    bool hatBall();
    /************************************************************
    * 
    * ? BohleBots::ballEntfernung()
    *   
    * * Beschreibung:
    *      gibt eine Balldistanz in folgendem Format an:
    *       15 = Ballstrahlung minimal             => weit weg
    *      ...
    *        5 = Ballstrahlung ca. 30% vom Maximum => mittel weit weg
    *      ...
    *        1 = BallStrahlung maximal             => nah
    *        0 = Ballstrahlung nicht messbar       => nicht da
    *        
    * ! Achtung:  
    *     bedarf Kalibrierung bei Programmstart
    *     einfach den Ball einmal vor Start in die Schale legen 
    *     (z.B. nachdem die Pixy eingestellt wurde)
    *     BETA:
    *       TODO: Werte glätten, da Entfernung bisher Flanken des Balls folgt
    *             (vgl. BohleBots Titan 
    *                   => Durchschnittswert der letzten n Messungen)
    * 
    * ! IR-Ring Firmware muss mindestens Version 10 sein !!
    *
    ************************************************************/
    int ballEntfernung();
    /************************************************************
    *
    * ? BohleBots::kompassRichtung()
    *   
    * * Beschreibung:
    *     gibt die Richtung des Kompasssensors
    *     von -180° bis +180° zurueck
    * 
    * ! Achtung:
    *     Wenn kein Kompass benutzt wird, 
    *     ist der Ruckgabewert 0;
    *
    ************************************************************/ 
    int kompassRichtung();
    /************************************************************
    *
    * ? BohleBots::setKompassHeading()
    *   
    * * Beschreibung:
    *     setzt die Richtung von 0° / vorne 
    *     auf die aktuelle Richtung
    * 
    * ! Achtung:
    *     Wenn kein Kompass benutzt wird, 
    *     tut diese Funktion nichts!
    *
    ************************************************************/ 
    void setKompassHeading();
    /************************************************************
    *
    *   BohleBots::siehtTor()
    *   
    *  * Beschreibung:
    *      gibt zurueck ob die Pixy gerade etwas als Tor erkennt
    * 
    *  ! Achtung:
    *     Wenn keine Pixy benutzt wird, 
    *     ist der Rueckgabewert false;
    *
    ************************************************************/
    bool siehtTor();
    /************************************************************
    *
    * ? BohleBots::torRichtung()
    *   
    * * Beschreibung:
    *      gibt die Richtung des Tores per Pixy in Pixeln zurueck
    *      hierbei wird die Mitte der erkannten Signatur genommen 
    *      und der x-Wert minus die halbe Bildbreite gerechnet
    * 
    *  ! Achtung:
    *     Wenn keine weder Kompass noch Pixy benutzt werden,
    *     ist der Rueckgabewert 0!
    *     Wenn keine Pixy, aber ein Kompass benutzt wird,
    *     ist der Rueckgabewert der der Kompassrichtung!
    *
    ************************************************************/
    int torRichtung();
    /************************************************************
    *
    * ? BohleBots::torEntfernung()
    *   
    * * Beschreibung:
    *      gibt die Entfernung des Tores per Pixy zurueck
    *      hierbei wird von der Oberkannte des Tores die Hoehe 
    *      abgezogen und das Ergebnis durch 4 geteilt
    * 
    * ! Achtung:
    *     Diese Funktion ist ein Relikt von aus dem Jahr 2017
    *     Team Bohlebots Quadro, daher keine Garantie auf 
    *     universelle Sinnhaftigkeit!
    *
    ************************************************************/
    int torEntfernung();
    /************************************************************
    *
    *   BohleBots::siehtEigenesTor()
    *   
    *  * Beschreibung:
    *      gibt zurueck ob die Pixy gerade etwas als eigenes Tor 
    *      erkennt
    * 
    *  ! Achtung:
    *     Wenn keine Pixy benutzt wird, 
    *     ist der Rueckgabewert false;
    *
    ************************************************************/
    bool siehtEigenesTor();
    /************************************************************
    *
    * ? BohleBots::eigenesTorRichtung()
    *   
    * * Beschreibung:
    *      gibt die Richtung des eigenen Tores per Pixy in Pixeln 
    *      zurueck hierbei wird die Mitte der erkannten Signatur 
    *      genommen und der x-Wert minus die halbe Bildbreite 
    *      gerechnet
    * 
    *  ! Achtung:
    *     Wenn keine weder Kompass noch Pixy benutzt werden,
    *     ist der Rueckgabewert 0!
    *     Wenn keine Pixy, aber ein Kompass benutzt wird,
    *     ist der Rueckgabewert der der Kompassrichtung!
    *
    ************************************************************/
    int eigenesTorRichtung();
    /************************************************************
    *
    * ? BohleBots::eigenesTorEntfernung()
    *   
    * * Beschreibung:
    *      gibt die Entfernung des eigenen Tores per Pixy zurueck
    *      hierbei wird von der Oberkannte des Tores die Hoehe 
    *      abgezogen und das Ergebnis durch 4 geteilt
    * 
    * ! Achtung:
    *     Diese Funktion ist ein Relikt von aus dem Jahr 2017
    *     Team Bohlebots Quadro, daher keine Garantie auf 
    *     universelle Sinnhaftigkeit!
    *
    ************************************************************/
    int eigenesTorEntfernung();
    /************************************************************
    *
    * ? BohleBots::input(int)
    *   
    * * Beschreibung:
    *     gibt den Analogwert des PINs an 
    *     INPUT1, INPUT2, INPUT3 oder INPUT4 zurueck
    *
    * * Paramter:
    *     nummer: 1 => INPUT1
    *             2 => INPUT2
    *             3 => INPUT3
    *             4 => INPUT4
    *
    * ! Achtung:
    *     Wenn nummer nicht 1, 2, 3 oder 4 ist,
    *     ist der Rueckgabewert 0!    
    *
    ************************************************************/ 
    int input(int nummer);
    /************************************************************
    *
    * ? BohleBots::taster(int,int)
    *   
    * * Beschreibung:
    *      gibt zurueck ob der Taster von einem bestimmten Device 
    *      mit Nummber 1 oder 2 gedrueckt ist
    *
    * * Paramter:
    *    device: Adresse die auf den Taster-Modulen als
    *           Binaerzahl eingeloetet wurde
    *    nummer: 1 => links oder 2 => rechts
    *    
    * ! Achtung:
    *     Wenn device < 0, device > 7 ist, 
    *     oder nummer weder 1 noch 2 ist, 
    *     ist der Rueckgabewert false!
    *  
    ************************************************************/
    bool taster(int device, int nummer);
    /************************************************************
    *
    * ? BohleBots::setLed(int,int,FARBE)
    *   
    * * Beschreibung:
    *      setzt eine LED auf einem Device und einer nummer 
    *      auf eine bestimmte Farbee
    *
    * * Paramter:
    *    device: Adresse die auf den Taster-Modulen als
    *            Binaerzahl eingeloetet wurde
    *    nummer: 1 => links oder 2 => rechts
    *    farbe:  AUS, GRUEN, ROT, GELB, BLAU, CYAN,
    *            MAGENTA oder WEISS
    *    
    * ! Achtung:
    *     Wenn device < 0, device > 7 ist, 
    *     oder nummer weder 1 noch 2 ist, 
    *     passiert nichts!
    *     Die LED behaelt solange ihre Farbe, 
    *     bis diese ueberschrieben oder auf AUS gesetzt wird!
    *  
    ************************************************************/
    void setLed(int device, int nummer, FARBE farbe);
    /************************************************************
    *
    * ? BohleBots::kick(int)
    *   
    * * Beschreibung:
    *      setzt den Kicker PORT fuer eine bestimmte Zeit HIGH,
    *      sodass der Roboter kickt
    *
    * * Paramter:
    *    zeit: Anzahl der Millisekunden, die der PORT HIGH 
    *         gesetzt wird
    *    
    * ! Achtung:
    *     Je hoeher zeit, desto haerter der Kick, 
    *     aber um brennende Roboter zu verhindern,
    *     wird der Kick auf maximal 40 Millisekunden begrenzt!
    *     Diese Funktion beinhaltet das einzige delay,
    *     dass im gesamten Programm verwendet werden sollte!
    *  
    ************************************************************/
    void kick(int zeit);
  //////////////////////////////////////////////////////////////////////////////
  // ---------------------------------------------------------------------------
  // ENDE DESSEN, WAS DER USER DIESES HEADERS WISSEN MUSS!
  // Ab hier folgen private Variablen und Funktionen.
  // ---------------------------------------------------------------------------
  //////////////////////////////////////////////////////////////////////////////
  private:
    //
    // Ports, Adressen und Zustaende
    //
    int  _tastLedID[8]    = {  0x20,  0x21,   0x22,  0x23,  0x24,  0x25,  0x26, 0x27 };
    bool _portena[8]      = { false, false, false, false, false, false, false, false };
    bool _taster1Array[8] = { false, false, false, false, false, false, false, false };
    bool _taster2Array[8] = { false, false, false, false, false, false, false, false };
    int  _led1Array[8]    = {     0,     0,     0,     0,     0,     0,     0,     0 };
    int  _led2Array[8]    = {     0,     0,     0,     0,     0,     0,     0,     0 };
    //
    // Flags und Variablen zur Zwischenspeicherung
    //
    // BotTyp
    int _botTyp = 0;
    // Ball
    bool _ballVorhanden        = false;
    int  _ballRichtung         = 0;
    int  _ballEntfernung       = 0;
    bool _hatBall              = false;
    int  _lichtPin             = 0;
    unsigned int _ballSchwelle = 500;
    // Kompass
    bool _benutztKompass       = false;
    int  _head                 = 0;
    int  _kompassWert          = 0;
    // Pixy
    Pixy2I2C pixy; 
    bool _benutztPixy          = false;
    int  _torSignatur          = 1; // Tor ist immer Signatur 1
    bool _siehtTor             = false;
    int  _torRichtung          = 0;
    int  _torEntfernung        = 0;
    int  _eigenesTorSignatur   = 2; // eigenes Tor ist immer Signatur 2
    bool _siehtEigenesTor      = false;
    int  _eigenesTorRichtung   = 0;
    int  _eigenesTorEntfernung = 0;    
    // Zeiten  
    elapsedMillis totZeit;    // CoolDown-Timer, damit der Kicker nicht ueberhitzt
    elapsedMillis warteZeit;  // Timer fuer die warte()-Funktion
    //
    // Funktionen
    //
    // I2C
    void i2csync();
    int  spdToPWM(int speed);
    void statusblink(FARBE farbe);   
    // Kompass
    int  kompass_org();
    int  kompass_lesen();
    // Pixy
    void pixy_auswerten();
    void pixy_read(); 
    // Motoren
    void motor(int number, int speed);
    void fahre3(int richtung, int tempo, int drehung);
    void fahre4(int richtung, int tempo, int drehung);
};
#endif // BOHLEBOTS_H