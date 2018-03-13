#include <DallasTemperature.h>
#include <SD.h>
#include <SPI.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <iarduino_OLED.h>
#include <Wire.h>
#include <Servo.h>
#include <EEPROM.h>

// -- Начало LCD Дисплей
LiquidCrystal_I2C lcd(0x27, 20, 4);
// -- Конец LCD Дисплей

// -- Начало RTC
RTC_DS1307 rtc;
// -- Конец RTC

// -- Начало SD
char logFileName[13];
// -- Конец SD

// -- Начало OLED Дисплей
iarduino_OLED oled(0x3C);
// -- Конец OLED Дисплей

// -- Начало температуры
#define THERMO_SENSOR_COUNT 4
#define THERMO_PIN 8
OneWire oneWire(THERMO_PIN);
DallasTemperature sensors(&oneWire);
// -- Конец температуры

// -- Начало Servo
Servo servoTap;
// -- Конеу Servo

// -- Начало Джойстик
unsigned int jButtons = 0;
// -- Конец Джойстик

// -- Начало меню

// -- Конец меню

void setup() {
  Serial.begin(9600);
  lcdSetup();
  oledSetap();
  thermoSetup();
  timeSetup();
  logSetup();
  servoSetup();
  controlSetup();
  lcd.clear();
}

void loop() {
  jButtons = 0;

  float* t = thermo();
  lcdTemperature(t, THERMO_SENSOR_COUNT);
  logThermo(t, THERMO_SENSOR_COUNT);
  graphPut(t[0]);

  control(3);

  oled.clrScr();
  menuLoop();
  oled.update();
}

