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
  cfgSetup();
  beepSetup();
  lcdSetup();
  oledSetap();
  thermoSetup();
  timeSetup();
  logSetup();
  servoSetup();
  controlSetup();
  compressorSetup();
  cookSetup();
  lcd.clear();
  beep(1);
}

void loop() {
  jButtons = 0;
  control(5);

  thermoLoop();
  Serial.println(thermoT0Int());
  Serial.println(thermoT0());
  Serial.println(thermoTC());
  Serial.println(thermoTT());
  Serial.println(thermoTOZ());

  graphLoop();

  calibrationLoop();

  prog1Loop();
  prog2Loop();
  cookLoop();

  logLoop();

  oledClean();
  menuLoop();
  oledUpdate();
}
