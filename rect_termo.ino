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

// -- Начало температуры
#define THERMO_SENSOR_COUNT 4
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
  graphPut(t[0]);
  control(1);
  
  calibrationLoop(t, THERMO_SENSOR_COUNT);
  control(1);
  
  lcdTemperature(t, THERMO_SENSOR_COUNT);
  control(1);
  
  logThermo(t, THERMO_SENSOR_COUNT);
  control(1);

  oledClean();
  menuLoop();
  oledUpdate();
  control(3);
}

