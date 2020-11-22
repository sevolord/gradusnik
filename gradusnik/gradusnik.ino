/**************************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

  This example is for a 128x32 pixel display using I2C to communicate
  3 pins are required to interface (two I2C and one reset).

  Adafruit invests time and resources providing this open
  source code, please support Adafruit and open-source
  hardware by purchasing products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries,
  with contributions from the open source community.
  BSD license, check license.txt for more information
  All text above, and the splash screen below must be
  included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SEALEVELPRESSURE_HPA (1013.25)

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;
OneWire ds1(2);
OneWire ds2(3);
OneWire ds3(4);


void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println("SSD1306 allocation failed");
    
  }
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    
  }
}

// Show initial display buffer contents on the screen --
// the library initializes this with an Adafruit splash screen.



void loop() {
  text();
}


void text(void) {
  byte data1[2];
  byte data2[2];
  byte data3[2];
  ds1.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds1.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство
  ds1.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
  ds2.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds2.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство
  ds2.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
  ds3.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds3.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство
  ds3.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
  delay(1000); // Микросхема измеряет температуру, а мы ждем.
  ds1.reset(); // Теперь готовимся получить значение измеренной температуры
  ds1.write(0xCC);
  ds1.write(0xBE); // Просим передать нам значение регистров со значением температуры
  ds2.reset(); // Теперь готовимся получить значение измеренной температуры
  ds2.write(0xCC);
  ds2.write(0xBE); // Просим передать нам значение регистров со значением температуры
  ds3.reset(); // Теперь готовимся получить значение измеренной температуры
  ds3.write(0xCC);
  ds3.write(0xBE); // Просим передать нам значение регистров со значением температуры
  // Получаем и считываем ответ
  data1[0] = ds1.read(); // Читаем младший байт значения температуры
  data1[1] = ds1.read(); // А теперь старший
  data2[0] = ds2.read(); // Читаем младший байт значения температуры
  data2[1] = ds2.read(); // А теперь старший
  data3[0] = ds3.read(); // Читаем младший байт значения температуры
  data3[1] = ds3.read(); // А теперь старший
  // Формируем итоговое значение:
  //    - сперва "склеиваем" значение,
  //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
  float temperature1 =  ((data1[1] << 8) | data1[0]) * 0.0625;
  float temperature2 =  ((data2[1] << 8) | data2[0]) * 0.0625;
  float temperature3 =  ((data3[1] << 8) | data3[0]) * 0.0625;



display.clearDisplay();
display.setTextSize(1); // Draw 2X-scale text
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 0);
display.print(F("BME:T="));
display.print(bme.readTemperature());
display.print(F(" H="));
display.print(bme.readHumidity());
//  display.setCursor(0, 8);
//  display.print(F("p= "));
//  display.print(bme.readPressure() / 100.0F);
//  display.setCursor(0, 16);
//  display.print(F(" a="));
//  display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
display.setCursor(0, 8);
display.print(F("DS1:T= "));
display.print(temperature1);

display.setCursor(0, 16);
display.print(F("DS2:T= "));
display.print(temperature2);

display.setCursor(0, 24);
display.print(F("DS3:T= "));
display.print(temperature3);
display.display();      // Show initial text

}

float temp(OneWire ds)
{
  byte data[2];
  ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство
  ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память

  delay(1000); // Микросхема измеряет температуру, а мы ждем.

  ds.reset(); // Теперь готовимся получить значение измеренной температуры
  ds.write(0xCC);
  ds.write(0xBE); // Просим передать нам значение регистров со значением температуры
  // Получаем и считываем ответ
  data[0] = ds.read(); // Читаем младший байт значения температуры
  data[1] = ds.read(); // А теперь старший
  // Формируем итоговое значение:
  //    - сперва "склеиваем" значение,
  //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
  float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
}
//
//void loop() {
//  Serial.print("Temperature = ");
//  Serial.print(int(bme.readTemperature()));
//  Serial.println("*C");
//
//  Serial.print("Pressure = ");
//  Serial.print(bme.readPressure() / 100.0F);
//  Serial.println("hPa");
//
//  Serial.print("Approx. Altitude = ");
//  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
//  Serial.println("m");
//
//  Serial.print("Humidity = ");
//  Serial.print(bme.readHumidity());
//  Serial.println("%");
//
//  Serial.println();
//  delay(1000);
//}
