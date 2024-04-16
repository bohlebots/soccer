// Hier kann bei bedarf die Slave-Adresse geändert werden
#define I2C_ADDRESS 0x55
uint8_t volatile i2cdata = 0; // Achtung : BYTE

RunningMedian<int,10> ballmedian;        // Es wird der Mendian der letzen 50 Werte genommen. Das verhinder das Flattern
RunningMedian<int,10> balldistmedian;    // Distanz                                    nachher für den Master nur Median

int tssp_pin[] = {19, 18, 05, 17, 04, 02, 15, 13, 14, 27, 26, 25, 33, 32, 35, 34};      //16 Stück 
int richtung[] = { 0, -1, -2, -3, -4, -5, -6, -7,  8,  7,  6,  5,  4,  3,  2,  1};
int wert[] =     { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};      //16 
// Reihenfolge: vorne Mitte, vorne links, links......rechts, vorne rechts
// "verkehrt" herum, weil Platine nachher auf dem Kopf

int wertmax;
int richtungmax;

int ballrichtung;
int ballmax;
int zone;
byte can_richtung;

elapsedMillis zeit;                               // kann mit zeit = 0 gesetzt werden und läuft dann rund 8 Minuten

int durchgaenge;
int zaehler=0;

void i2c_transmit() 
{
  if (ballmedian.getStatus()     == ballmedian.OK)     { ballmedian.getMedian(ballrichtung);   }
  if (balldistmedian.getStatus() == balldistmedian.OK) { balldistmedian.getMedian(zone);     }
  i2cdata=zone*16+ballrichtung;  // Zone 0-16 und Ballrichtung 0-16
  Wire.write(i2cdata);
  Serial.println("x");
}

// Called when the I2C slave gets written to
void i2c_recv(int len) 
{
  //digitalWrite(LED_BUILTIN, HIGH);
  // Just read and forget the sent bytes
  for (int i = 0; i < len; i++) { int r = Wire.read(); }
  //delay(1);
  //digitalWrite(LED_BUILTIN, LOW);
  Serial.println(".");
}

void setup()
{          
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  for (int lauf = 0; lauf<16; lauf++) pinMode(tssp_pin[lauf],INPUT);
    
  Wire.onRequest(i2c_transmit);     // i2c interrupt transmit - liefert die Ergebnisse der Messung
  Wire.onReceive(i2c_recv);
  Wire.begin((uint8_t)I2C_ADDRESS); 
  
  #if CONFIG_IDF_TARGET_ESP32
  char message[64];
  uint32_t i = 0;
  snprintf(message, 64, "%u Packets.", i++);
  Wire.slaveWrite((uint8_t *)message, strlen(message));
  Serial.println("los");

#endif
}  // end of setup()
