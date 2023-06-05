// Hier kann bei bedarf die Slave-Adresse geändert werden
const byte PICO_I2C_ADDRESS = 0x55;
const byte PICO_SDA = 20;
const byte PICO_SCL = 21;
int volatile i2cdata = 0;

RunningMedian<int,50> ballmedian;        // Es wird der Mendian der letzen 50 Werte genommen. Das verhinder das Flattern

int tssp_pin[] = {15, 16, 17, 18, 19, 22, 26, 28,  8,  9, 10, 11, 12, 13, 14};      //15 Stück 
int richtung[] = { 0,  0,  1,  2,  3,  4,  6,  8,  8, -6, -4, -3, -2, -1,  0};
int wert[] =     { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};      //15 
// Reihenfolge: vorne Mitte, vorne rechts, rechts......links, vorne links

int distanz_teiler[] = {
  8, // richtung 0
  6, 6, 5, 
  4, // richtung 2
  4, // richtung 4
  3, // richtung 6
  4, 4, // richtung 8
  3, // richtung -6
  4, // richtung -4
  4, // richtung -2
  5, 6, 6
};

int wertmax;
int richtungmax;


int ballrichtung;
int ballmax;
int distanz;
byte can_richtung;

elapsedMillis zeit;                               // kann mit zeit = 0 gesetzt werden und läuft dann rund 8 Minuten

int durchgaenge;
int zaehler=0;

void i2c_transmit() 
{
  if (ballmedian.getStatus() == ballmedian.OK)     { ballmedian.getMedian(ballrichtung);   }
  i2cdata = distanz * 16 + (ballrichtung + 7);
  Wire.write(i2cdata);
}

// Called when the I2C slave gets written to
void i2c_recv(int len) 
{
  //digitalWrite(LED_BUILTIN, HIGH);
  // Just read and forget the sent bytes
  for (int i = 0; i < len; i++) { int r = Wire.read(); }
  //delay(1);
  //digitalWrite(LED_BUILTIN, LOW);
}

void setup()
{          
  //pinMode(LED_BUILTIN, OUTPUT);  
  Serial.begin(115200);
  for (int lauf = 0; lauf<15; lauf++) pinMode(tssp_pin[lauf],INPUT);
  Wire.setSDA(PICO_SDA);
  Wire.setSCL(PICO_SCL);
  Wire.begin(PICO_I2C_ADDRESS);     // join i2c bus as slave
  Wire.onRequest(i2c_transmit);     // i2c interrupt transmit - liefert die Ergebnisse der Messung
  Wire.onReceive(i2c_recv);
  
}  // end of setup()
