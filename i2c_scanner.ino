// --------------------------------------
// i2c_scanner
//
// 
/*
 ### OLED ###
 OLED 128x64 0x3C 
 https://github.com/adafruit/Adafruit_SSD1306
 https://github.com/adafruit/Adafruit-GFX-Library
 noch mit hinzu installieren entwerder mit Hand oder mit Str + Umschalt + i Bibliothekenverwaltung
 Arduino China Clone Pro Micro, ist Leonardo kompatibel.
 
  Arduino Uno, Mega                                 0x01 - 0x05 (eigene Festlegung)
  Attiny45/85                                            0x06 - 0x09 (eigene Festlegung)
  Temperatursensor MCP9808                   0x18 - 0x1F
  Beschleunigungssensor MMA8451          0x1D, 0x1C
  Portexpander MCP23017 / MCP23S17    0x20 - 0x27
  Portexpander PCF8574N / PCF8574P      0x20 - 0x27
  LCD 20x4 mit PCF8574T                         0x27 (fest eingestellt?)
  LCD 16x2 mit PCF8574AT                       0x3F (über Lötbr. einstellbar 0x38 - 0x3F)
  Portexpander PCF8574AN                       0x38 - 0x3F
  RGB- u. Gestik-Sensor APDS-9930         0x39
  Stromsensor INA219                               0x40 - 0x4F
  Stromsensor INA219                               0x40, 0x41, 0x44, 0x45 (auf Adafruit Breakout)
  Luftfeuchtesensor SHT3x-D                    0x44, 0x45
  ADC ADS1015, ADS1115                         0x48 - 0x4B
  EEPROM AT24C32                                   0x50 (auf DS1307 Breakout)
  EEPROM AT24C32                                   0x57 (auf DS3231 Breakout, einstellbar über Lötbr.)
  EEPROM z.B. AT24C32/64                       0x50 - 0x57
  EEPROM z.B. 24LC024, 24LC256             0x50 - 0x57
  FRAM z.B. MP85RC256V                          0x50 - 0x57
  DAC MCP4725                                         0x62, 0x63
  RTC DS1307                                            0x68
  RTC DS3231                                            0x68
  I2C-Multiplexer TCA9548A                      0x70 - 0x77
  7-Segm.Anz. Sparkfun COM-1144x        0x71 (0x01 - 0x7E softwaremäßig einstellbar)
  Luftdruck u. -feuchtesensor BME280       0x76, 0x77
  Luftdrucksensor BMP280                         0x76, 0x77
  Luftdrucksensor BMP280                         0x77, 0x76 (auf Breakout von Adafruit)
  Luftdrucksensor BMP180                         0x77
  Luftfeuchtesensor AM2320                      0xB8  1)
  Mehr Infos: https://arduino-projekte.webnode.at/i2c-scanner
*/
#include <Wire.h>
byte I2CAdd;
byte fehler;
byte anzahl100 = 0;
byte anzahl400 = 0;

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}


void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(2000);           // wait 5 seconds for next scan
  
  extrascan();
  
}

void extrascan()
{
  Serial.println("\nStarte Scan (100 kHz) ... ");
  Wire.setClock(100000L);
  for (I2CAdd = 1; I2CAdd < 128; I2CAdd++)
  {
    Wire.beginTransmission(I2CAdd);
    fehler = Wire.endTransmission();
    if (fehler == 0)
    {
      anzahl100++;
      Serial.print("Baustein mit Adresse 0x");
      Serial.print(I2CAdd, HEX);
      Serial.println(" gefunden!");
    }
  }

  if (anzahl100 > 0)
  {
    Serial.println("\nStarte Scan (400 kHz) ... ");
    Wire.setClock(400000L);
    for (I2CAdd = 1; I2CAdd < 128; I2CAdd++)
    {
      Wire.beginTransmission(I2CAdd);
      fehler = Wire.endTransmission();
      if (fehler == 0)
      {
        anzahl400++;
        Serial.print("Baustein mit Adresse 0x");
        Serial.print(I2CAdd, HEX);
        Serial.println(" gefunden!");
      }
    }
    Serial.println("\nScan beendet!");
    Serial.print("Anzahl gefunden 100/400 kHz: ");
    Serial.print(anzahl100);
    Serial.print("/");
    Serial.print(anzahl400);
    Serial.println(" Baustein(e)");
  }
  else Serial.println("\nKeinen I2C-Baustein gefunden!");

  anzahl100 = 0;
  anzahl400 = 0;
  Serial.println(); Serial.println();
  delay(10000);
}
