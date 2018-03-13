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

// -- Начало меню
const byte MENU_SIZE = 2;
char* menuList[] = {
  "График",
  "Дата/время",
};
byte menuSelected = 0;
byte menuActive = 255;
// -- Конец меню

void setup() {
  Serial.begin(9600);

  // -- Начало загрузки LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  printLog("LCD Loaded");
  // -- Конец загрузки LCD

  // -- Начало загрузки LCD
  printLog("OLED loading...");
  oled.begin();           // Инициируем работу с дисплеем.
  oled.setFont(SmallFontRus);
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
    menuActive = 1;
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
  jButtons = 0;

  sensors.requestTemperatures();
  float t = 0;
  for (byte i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
    t = sensors.getTempC(thermometer[i]);
    lcd.setCursor(0, i);
    lcd.print("     ");
    lcd.setCursor(0, i);
    lcd.print(t);
  }
  putTemp(t);

  readControl(3);

  oled.clrScr();
  if (menuActive == 0) {
    menu0();
  } else if (menuActive == 1) {
    menu1();
  } else {
    menu();
  }
  oled.update();

  lcd.setCursor(10, 0);
  lcd.print("   ");
  lcd.setCursor(10, 0);
  lcd.print(menuActive);

  lcd.setCursor(10, 1);
  lcd.print("   ");
  lcd.setCursor(10, 1);
  lcd.print(menuSelected);
}

void readControl(byte t) {
  int s;
  bool beep = false;
  for (byte i = 0; i < t; i++) {
    s = analogRead(J_X);
    if (s < 256) {
      bitSet(jButtons, 12);
      beep = true;
    } else if (s > 768) {
      bitSet(jButtons, 10);
      beep = true;
    }
    s = analogRead(J_Y);
    if (s < 256) {
      bitSet(jButtons, 13);
      beep = true;
    } else if (s > 768) {
      bitSet(jButtons, 11);
      beep = true;
    }
    if (digitalRead(J_B) == LOW) {
      bitSet(jButtons, 14);
      beep = true;
    }
    delay(100);
  }
  if (beep) {
    tone(BUZZER_PIN, 2000, 100);
  }
}

void menu() {
  if (MENU_SIZE <= 5) {
    for (byte i = 0; i < MENU_SIZE; i++) {
      if (menuSelected == i) {
        oled.print(">", 5, 10 * (i + 1));
      }
      oled.print(menuList[i], 10, 10 * (i + 1));
    }
  } else {
    oled.print(">", 5, 30);
    for (byte i = 0; i < MENU_SIZE; i++) {
      byte j = nextCycleValue(0, MENU_SIZE - 1, i + menuSelected - 2);
      oled.print(menuList[j], 10, 10 * (i + 1));
    }
  }
  // Контроль
  if (bitRead(jButtons, 10)) {
    menuSelected++;
    if (menuSelected > MENU_SIZE - 1) {
      menuSelected = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    if (menuSelected <= 0) {
      menuSelected = MENU_SIZE - 1;
    } else {
      menuSelected--;
    }
  }
  if (bitRead(jButtons, 14)) {
    menuActive = menuSelected;
  }
}

void putTemp(float t) {
  graphI++;
  if (graphI >= GRAPH_SIZE) {
    graphI = 0;
  }
  graph[graphI] = (int) (t * 100);
}

void menu0() {
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
  // Конец текст

  // -- Начало контроля
  short a = 0;
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
  }
  if (bitRead(jButtons, 12)) {
    graphMin += a;
  }
  if (bitRead(jButtons, 11) && graphZoom < 16) {
    graphMin += 64 / graphZoom * 25;
    graphMin -= graphMin % 100;
    graphZoom = graphZoom * 2;
  }
  if (bitRead(jButtons, 13) && graphZoom > 1) {
    graphZoom = graphZoom / 2;
    graphMin -= 64 / graphZoom * 25;
    graphMin -= graphMin % 100;
  }
  if (bitRead(jButtons, 14)) {
    menuActive = 255;
  }
  // -- Конец контроля
}

int newTime[6] = {2018, 6, 15, 12, 30, 0};
byte newTimeI = 0;
void menu1() {
  oled.print(menuList[1], 5, 10);
  oled.print(getTime(), 5, 24);

  char newTimeStr[19];
  sprintf(newTimeStr, "%04d-%02d-%02d %02d:%02d:%02d", newTime[0], newTime[1], newTime[2], newTime[3], newTime[4], newTime[5]);
  newTimeStr[19] = '\0';

  oled.print(newTimeStr, 5, 34);

  char* newTimeName[] = {
    "Год",
    "Месяц",
    "День",
    "Час",
    "Минута",
    "Секунда",
    "Сохранить",
    "Выход",
  };

  oled.print(newTimeName[newTimeI], 5, 44);

  if (bitRead(jButtons, 10)) {
    if (newTimeI < 6) {
      newTime[newTimeI]++;
      if (newTimeI == 1 && newTime[newTimeI] > 12) {
        newTime[newTimeI] = 1;
      } else if (newTimeI == 2 && newTime[newTimeI] > 31) {
        newTime[newTimeI] = 1;
      } else if (newTimeI == 3 && newTime[newTimeI] > 23) {
        newTime[newTimeI] = 0;
      } else if ((newTimeI == 4 || newTimeI == 5) && newTime[newTimeI] > 59) {
        newTime[newTimeI] = 0;
      }
    }
  }
  if (bitRead(jButtons, 11)) {
    newTimeI++;
    if (newTimeI > 7) {
      newTimeI = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    if (newTimeI < 6) {
      newTime[newTimeI]--;
      if (newTimeI == 0 && newTime[newTimeI] < 2000) {
        newTime[newTimeI] = 2000;
      } else if (newTimeI == 1 && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 12;
      } else if (newTimeI == 2 && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 31;
      } else if (newTimeI == 3 && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 23;
      } else if ((newTimeI == 4 || newTimeI == 5) && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 59;
      }
    }
  }
  if (bitRead(jButtons, 13)) {
    if (newTimeI <= 0) {
      newTimeI = 6;
    } else {
      newTimeI--;
    }
  }
  if (bitRead(jButtons, 14)) {
    if (newTimeI == 7) {
      rtc.adjust(DateTime(newTime[0], newTime[1], newTime[2], newTime[3], newTime[4], newTime[5]));
    } else {
      menuActive = 255;
    }
  }
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

char nextCycleValue(char minV, char maxV, char i) {
  while (i > (minV - maxV)) {
    i -= (maxV - minV) + 1;
  }
  while (i < 0) {
    i += (maxV - minV) + 1;
  }
  return minV + i;
}

