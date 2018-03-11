#include <DallasTemperature.h>
#include <SD.h>
#include <SPI.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <iarduino_OLED.h>
#include <Wire.h> 

// -- Начало LCD Дисплей
LiquidCrystal_I2C lcd(0x27, 20, 4);
byte logI = 0;
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
extern uint8_t SmallFont[];
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
short graph[GRAPH_SIZE];
byte graphI = 0;
byte graphZoom = 4;
short graphMin = 2000;
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

void setup() {
  Serial.begin(9600);

  // -- Начало загрузки LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  printLog("LCD Loaded");
  // -- Конец загрузки LCD

  // -- Начало загрузки LCD
  printLog("OLED loading...");
  oled.begin();           // Инициируем работу с дисплеем.
  oled.setFont(SmallFont);
  oled.autoUpdate(false); // Запрещаем автоматический вывод данных. Информация на дисплее будет обновляться только после обращения к функции update().
  oled.clrScr();
  oled.print("OLED", 0, 7);
  oled.update();
  printLog("OLEDed loaded");
  // -- Конец загрузки LCD

  // -- Начало загрузки LCD
  printLog("Sensors loading...");
  sensors.begin();
  for (int i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    sensors.setResolution(thermometer[i], TEMPERATURE_PRECISION);
  }
  printLog("Sensors loaded");
  // -- Конец загрузки LCD

  // -- Начало загрузки RTC
  printLog("RTC loading...");
  if (!rtc.begin()) {
    printLog("RTC Error");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(2018, 1, 1, 0, 0, 0));
  }
  printLog(getTime());
  printLog("RTC loaded");
  // -- Конец загрузки RTC

  // -- Начало загрузки SD
  printLog("SD loading...");
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_PIN)) {
    printLog("SD Error");
    while (1);
  }
  DateTime now = rtc.now();
  sprintf(logFileName, "%04d%02d%02d.txt", now.year(), now.month(), now.day());
  logFileName[12] = '\0';
  printLog(logFileName);
  File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) {
    logFile.println(getTime());
    logFile.close();
  } else {
    printLog("File open Error");
    while (1);
  }
  printLog("SD loaded");
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
  sensors.requestTemperatures();
  float t = 0;
  for (byte i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    t = sensors.getTempC(thermometer[i]);
    lcd.setCursor(0, i);
    lcd.print(t);
  }
  putTemp(t);
  printGraph();
  pause(10);
  joystick();
}

void pause(byte t) {
  int s;
  for (byte i = 0; i < t; i++) {
    s = analogRead(J_X);
    if (s < 256) {
      bitSet(jButtons, 12);
    } else if (s > 768) {
      bitSet(jButtons, 10);
    }
    s = analogRead(J_Y);
    if (s < 256) {
      bitSet(jButtons, 13);
    } else if (s > 768) {
      bitSet(jButtons, 11);
    }
    delay(100);
  }
}

void joystick() {
  short a = 0;
  bool beep = false;
  if (graphZoom == 1) {
    a = 400;
  } else if (graphZoom == 2) {
    a = 200;
  } else if (graphZoom == 4) {
    a = 100;
  } else {
    a = 50;
  }
  if (bitRead(jButtons, 10)) {
    graphMin -= a;
    beep = true;
  }
  if (bitRead(jButtons, 12)) {
    graphMin += a;
    beep = true;
  }
  if (bitRead(jButtons, 11) && graphZoom < 16) {
    graphMin += 64 / graphZoom * 25;
    graphMin -= graphMin % 100;
    graphZoom = graphZoom * 2;
    beep = true;
  }
  if (bitRead(jButtons, 13) && graphZoom > 1) {
    graphZoom = graphZoom / 2;
    graphMin -= 64 / graphZoom * 25;
    graphMin -= graphMin % 100;
    beep = true;
  }
  jButtons = 0;
  if (beep) {
    tone(BUZZER_PIN, 2000, 100);
  }
}

void putTemp(float t) {
  graphI++;
  if (graphI >= GRAPH_SIZE) {
    graphI = 0;
  }
  graph[graphI] = (int) (t * 100);
}

void printGraph() {
  oled.clrScr();
  // Начало график
  byte x, y;
  short graphMax = graphMin + 64 / graphZoom * 100;
  for (byte i = 0; i < GRAPH_SIZE; i++) {
    if (i > graphI) {
      x = GRAPH_SIZE - i + graphI;
    } else {
      x = graphI - i;
    }
    x += 16;
    y = map(graph[i], graphMin, graphMax, 63, 0);
    oled.drawPixel(x, y, true);
  }
  // Конец график
  // Начало сетка
  short dt = 500;
  if (graphZoom == 1) {
    dt = 1000;
  } else if (graphZoom == 2) {
    dt = 500;
  } else if (graphZoom == 2) {
    dt = 500;
  } else if (graphZoom == 4) {
    dt = 500;
  } else if (graphZoom == 8) {
    dt = 200;
  } else {
    dt = 100;
  }
  short t;
  if (graphMin % dt == 0) {
    t = graphMin;
  } else {
    t = graphMin - graphMin % dt + dt;
  }
  for (; t < graphMax; t += dt) {
    for (byte i = 16; i < 128; i += 5) {
      y = map(t, graphMin, graphMax, 63, 0);
      oled.drawPixel(i, y, true);
      oled.print((t / 100) % 100, 0, y + 4);
    }
  }
  // Конец сетка
  // Начало текст
  oled.drawRect(0, 0, 127, 9, true , false);
  oled.print("t", 0, 7);
  oled.print(((float) graph[graphI]) / 100, 6, 7);
  oled.print("min", 50, 7);
  oled.print(((float) graphMin) / 100, 69, 7);
  oled.print("x", 105, 7);
  oled.print(graphZoom, 111, 7);
  oled.update();
  // Конец текст
}

void printLog(String str) {
  Serial.println(str);
  logI++;
  if (logI >= 4) {
    logI = 0;
  }
  lcd.setCursor(0, logI);
  lcd.print("                    ");
  lcd.setCursor(0, logI);
  lcd.print(str);
  delay(1000);
}

String getTime() {
  char r[19];
  DateTime now = rtc.now();
  sprintf(r, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  r[19] = '\0';
  return r;
}

