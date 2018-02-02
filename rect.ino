#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <iarduino_OLED_txt.h> 
#include <LiquidCrystal_I2C.h>

#define PIN_TEMPERATURE 8
#define PIN_SD 10
#define TEMPERATURE_PRECISION 11
#define TEMPERATURE_SENSOR_COUNT 4
#define GRAPH_SIZE 120

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(PIN_TEMPERATURE);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

RTC_DS1307 rtc; // Определяем Real Time Clock объект
DateTime now;

Servo servo;

LiquidCrystal_I2C lcd(0x27, 20, 4); // Для экрана 20х4 (четырехстрочный)
iarduino_OLED_txt oled(0x3C);
extern uint8_t SmallFontRus[];

File dataFile;
char dataFileName[12];
//  ROM = 28 FF AA B5 C1 17 4 CD
//    Chip = DS18B20
//    Data = 1 8C 1 4B 46 7F FF C 10 58  CRC=58
//    Temperature = 24.75 Celsius, 76.55 Fahrenheit
//  ROM = 28 FF 99 E3 C1 17 5 88
//    Chip = DS18B20
//    Data = 1 8C 1 4B 46 7F FF C 10 58  CRC=58
//    Temperature = 24.75 Celsius, 76.55 Fahrenheit
//  ROM = 28 FF A5 EC C1 17 5 B7
//    Chip = DS18B20
//    Data = 1 8B 1 4B 46 7F FF C 10 88  CRC=88
//    Temperature = 24.69 Celsius, 76.44 Fahrenheit
//  No more addresses.

DeviceAddress thermometer[4] = 
{
  { 0x28, 0xFF, 0x10, 0xD1, 0xC1, 0x17, 0x4, 0xA1 },
  { 0x28, 0xFF, 0xAA, 0xB5, 0xC1, 0x17, 0x4, 0xCD },
  { 0x28, 0xFF, 0x99, 0xE3, 0xC1, 0x17, 0x5, 0x88 },
  { 0x28, 0xFF, 0xA5, 0xEC, 0xC1, 0x17, 0x5, 0xB7 }
};
float temperature[TEMPERATURE_SENSOR_COUNT];
float graph[GRAPH_SIZE];
int graphI = 0;

void setup() {
  while (!Serial);
  
  Serial.begin(9600);
  Serial.println("RTC loading...");
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //rtc.adjust(DateTime(2018, 1, 31, 20, 45, 0));
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT runing!");
  }
  Serial.println("RTC loaded");
  
  Serial.println("LCD loading...");
  lcd.begin();
  lcd.setCursor(0, 0); // 1 строка
  lcd.print("LCD I2C Test - 20x4");
  lcd.setCursor(0, 1); // 2 строка
  lcd.print("01234567899876543210");
  lcd.setCursor(0, 2); // 3 строка
  lcd.print("01234567899876543210");
  lcd.setCursor(0, 3); // 4 строка
  lcd.print("  CHINGACHGOOK.NET");
  Serial.println("LCD loaded");
  
  Serial.println("OLED loading...");
  oled.begin();
  oled.setFont(SmallFontRus);
  oled.setCursor(16,4);
  oled.print("OLED");
  
  Serial.println("OLED loaded");

  Serial.println("SD loading...");
  if (!SD.begin(PIN_SD)) {
    Serial.println("Card failed, or not present");
    return;
  }
  now = rtc.now();
  sprintf(dataFileName, "%04d%02d%02d.txt", now.year(), now.month(), now.day());
  Serial.print("File: ");
  Serial.println(dataFileName);
  // Текущая дата из часов реального времени
  dataFile = SD.open(dataFileName, FILE_WRITE);
  dataFile.print("Начало логирования: ");
  dataFile.print(now.hour());
  dataFile.print(":");
  dataFile.print(now.minute());
  dataFile.print(":");
  dataFile.print(now.second());
  dataFile.println();
  dataFile.close();
  Serial.println("SD loaded");
  
  Serial.println("Sensors loading...");
  sensors.begin();
  // locate devices on the bus
  /*Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");*/
  // report parasite power requirements
  /*Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");*/
  // method 1: by index
  /*for (int i = 0; i < 4; i++) {
    if (!sensors.getAddress(thermometer[i], i)){
      Serial.println("Unable to find address for Device ");
      Serial.println(i);
    }
  }*/
  // show the addresses we found on the bus
  /*for (int i = 0; i < 4; i++) {
    Serial.print("Device ");
    Serial.print(i);
    Serial.print(" Address: ");
    printAddress(thermometer[i]);
    Serial.println();
  }*/
  for (int i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    sensors.setResolution(thermometer[i], TEMPERATURE_PRECISION);
  }
  /*for (int i = 0; i < 4; i++) {
    Serial.print("Device ");
    Serial.print(i);
    Serial.print(" Resolution: ");
    printAddress(sensors.getResolution(thermometer[i]));
    Serial.println();
  }*/
  Serial.println("Sensors loaded");
}

void loop() {
  sensors.requestTemperatures();
  for (int i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    temperature[i] = sensors.getTempC(thermometer[i]);
  }
  now = rtc.now();
  dataFile = SD.open(dataFileName, FILE_WRITE);
  for (int i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    Serial.print(i, DEC);
    Serial.print(" ");
    Serial.println(temperature[i], DEC);
    dataFile.print(temperature[i], DEC);
    dataFile.print("  ");
  }
  dataFile.print(now.hour());
  dataFile.print(":");
  dataFile.print(now.minute());
  dataFile.print(":");
  dataFile.print(now.second());
  dataFile.println();
  dataFile.close();
  Serial.println("data stored");

  delay(1000);
}

// Напечатать адрес термодатчика
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}




