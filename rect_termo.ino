#include <DallasTemperature.h>
#include <SD.h>
#include <SPI.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <iarduino_OLED.h>
#include <Wire.h>

// -- Начало LCD Дисплей
LiquidCrystal_I2C lcd(0x27, 20, 4);
// -- Конец LCD Дисплей

// -- Начало RTC
RTC_DS1307 rtc;
// -- Конец RTC

// -- Начало SD
#define SD_PIN 53
char logFileName[13];
// -- Конец SD

// -- Начало OLED Дисплей
iarduino_OLED oled(0x3C);
extern uint8_t SmallFontRus[];
// -- Конец OLED Дисплей

// -- Начало температуры
#define GRAPH_SIZE 128
#define PIN_TEMPERATURE 8
#define TEMPERATURE_PRECISION 11
#define TEMPERATURE_SENSOR_COUNT 4
DeviceAddress thermometer[4] =
{
  { 0x28, 0xFF, 0x10, 0xD1, 0xC1, 0x17, 0x4, 0xA1 },
  { 0x28, 0xFF, 0xAA, 0xB5, 0xC1, 0x17, 0x4, 0xCD },
  { 0x28, 0xFF, 0x99, 0xE3, 0xC1, 0x17, 0x5, 0x88 },
  { 0x28, 0xFF, 0xA5, 0xEC, 0xC1, 0x17, 0x5, 0xB7 }
};
OneWire oneWire(PIN_TEMPERATURE);
DallasTemperature sensors(&oneWire);
// -- Конец температуры

// -- Начало Джойстик
const int J_X = A1;
const int J_Y = A0;
const int J_B = 2;
// 0 - 9 - цифры
// 10 - вверх
// 11 - вправо
// 12 - вниз
// 13 - влево
// 14 - Ok
// 15 - cancel
unsigned int jButtons = 0;
// -- Конец Джойстик

// -- Начало Пищалка
const int BUZZER_PIN = 3; //объявляем переменную с номером пина, на который мы
// -- Конец Пищалка

// -- Начало меню
byte menuActive = 255;
// -- Конец меню

void setup() {
  Serial.begin(9600);

  // -- Начало загрузки LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcdLog("LCD Loaded");
  // -- Конец загрузки LCD

  // -- Начало загрузки OLED
  lcdLog("OLED loading...");
  oled.begin();           // Инициируем работу с дисплеем.
  oled.setFont(SmallFontRus);
  oled.autoUpdate(false); // Запрещаем автоматический вывод данных. Информация на дисплее будет обновляться только после обращения к функции update().
  oled.clrScr();
  oled.print("OLED", 0, 7);
  oled.update();
  lcdLog("OLEDed loaded");
  // -- Конец загрузки OLED

  // -- Начало загрузки sensors
  lcdLog("Sensors loading...");
  sensors.begin();
  for (int i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    sensors.setResolution(thermometer[i], TEMPERATURE_PRECISION);
  }
  lcdLog("Sensors loaded");
  // -- Конец загрузки sensors

  // -- Начало загрузки RTC
  lcdLog("RTC loading...");
  if (!rtc.begin()) {
    lcdLog("RTC Error");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(2018, 1, 1, 0, 0, 0));
    menuActive = 1;
  }
  lcdLog(timeStr());
  lcdLog("RTC loaded");
  // -- Конец загрузки RTC

  // -- Начало загрузки SD
  lcdLog("SD loading...");
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_PIN)) {
    lcdLog("SD Error");
    while (1);
  }
  DateTime now = rtc.now();
  sprintf(logFileName, "%04d%02d%02d.txt", now.year(), now.month(), now.day());
  logFileName[12] = '\0';
  lcdLog(logFileName);
  File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) {
    logFile.close();
  } else {
    lcdLog("File open Error");
    while (1);
  }
  lcdLog("SD loaded");
  // -- Конец загрузки SD

  // -- Начало загрузки Джойстик
  pinMode(J_X, INPUT);
  pinMode(J_Y, INPUT);
  pinMode(J_B, INPUT_PULLUP);
  // -- Конец загрузки Джойстик

  // -- Начало загрузки Пищалка
  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 2000, 100);
  // -- Конец загрузки Пищалка

  lcd.clear();
}

void loop() {
  jButtons = 0;

  sensors.requestTemperatures();
  float t[TEMPERATURE_SENSOR_COUNT];
  for (byte i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    t[i] = sensors.getTempC(thermometer[i]);
  }
  lcdTemperature(t, TEMPERATURE_SENSOR_COUNT);
  
  graphPut(t[0]);

  control(3);

  oled.clrScr();
  if (menuActive == 0) {
    graphMenu();
  } else if (menuActive == 1) {
    timeMenu();
  } else {
    menu();
  }
  oled.update();
}

