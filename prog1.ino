#define PROG1_MAX_STEP 9

byte prog1StepI = 255;
byte prog1MenuStepI = 0;
unsigned long prog1StepStartTime = 0;
String prog1StepStartTimeStr = "";



char* prog1[] = {
  "\202\353\252\253\356\347\245\255\256"
  "\202\252\253\356\347\245\255\250\245",
  "\220\240\247\256\243\340\245\242",
  "\216\342\241\256\340 \243\256\253\256\242",
  "\216\342\241\256\340 \342\245\253\240 1",
  "\216\342\241\256\340 \342\245\253\240 2",
  "\216\342\241\256\340 \345\242\256\341\342\256\242",
  "\216\345\253\240\246\244\245\255\250\245",
  "\202\341\245 \341\244\245\253\240\255\256",
};

void prog1Menu() {
  menuTitle(5);
  oledPrint("\235\342\240\257", 28, 30, 1);
  oledPrintNl(prog1[prog1MenuStepI], 40);

  // Контроль
  if (bitRead(jButtons, 10)) {
    if (prog1MenuStepI <= 0) {
      prog1MenuStepI = PROG1_MAX_STEP;
    }
    prog1MenuStepI--;
  }
  if (bitRead(jButtons, 12)) {
    prog1MenuStepI++;
    if (prog1MenuStepI >= PROG1_MAX_STEP - 1) {
      prog1MenuStepI = 0;
    }
  }
  if (bitRead(jButtons, 14)) {
    prog1StepI = prog1MenuStepI;
    menuOpen(255);
  }
}

void prog1Loop(float t0, float tTank, float tCooler, float tOz) {
  if (prog1Step <= 0 || prog1Step >= PROG1_MAX_STEP) {
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print(prog1[prog1StepI]);

  lcd.setCursor(1, 0);
  lcd.print("                    ");

  lcd.setCursor(2, 0);
  lcd.print("TO");
  lcd.setCursor(2, 3);
  lcd.print(t0);

  lcd.setCursor(3, 0);
  lcd.print("TT");
  lcd.setCursor(3, 3);
  lcd.print(tTank);

  lcd.setCursor(2, 10);
  lcd.print("TC");
  lcd.setCursor(2, 13);
  lcd.print(tCooler);

  lcd.setCursor(3, 10);
  lcd.print("OZ");
  lcd.setCursor(3, 13);
  lcd.print(tOz);


  if (tOz > cfgOzMax()) {
    lcd.setCursor(1, 0);
    lcd.print(" !!! \216\206 \243\256\340\357\347\240\357 !!!");
    controlBeep(10);
  }

  if (tOz > cfgOzMax() + 10) {
    cookOff();
    prog1Step(7);
  }

  switch (prog1StepI) {
    case 1:
      prog1Step1();
      break;
    case 2:
      prog1Step2(t0);
      break;
    case 3:
      prog1Step3();
      break;
    case 4:
      prog1Step4(t0);
      break;
    case 5:
      prog1Step5(t0);
      break;
    case 6:
      prog1Step6(t0);
      break;
    case 7:
      prog1Step7();
      break;
    case 8:
      prog1Step8();
      break;
  }
}

void prog1Step(byte i) {
  prog1StepI = i;
  servoAdd(180); // Закрыть охлаждение
  controlBeep(i);
  prog1StepStartTime = millis();
  prog1StepStartTimeStr = timeStr();
}

// Включение
void prog1Step1() {
  cookOn(); // Включаем плиту
  prog1Step(2);
}

// Разогрев
void prog1Step2(float t0) {
  if (t0 > cfgTHeadStart()) {
    compressorOn(); // Включаем компрессор
    cookGood(); // Выставляем температуру на плите
    prog1Step(3);
  }
}

// Отбор голов
void prog1Step3() {
  if (millis() - prog1StepStartTime > ((unsigned long) cfgTTailStart()) * 60000) {
    prog1Step(4);
  }
}

// Отбор тела 1
void prog1Step4(float t0) {
  if (t0 > cfgT0()) {
    prog1Step(5);
  }
}

// Отбор тела 2
void prog1Step5(float t0) {
  float t0Normal = cfgT0();
  float t0Delta = cfgT0Delta();

  if (t0 < t0Normal - t0Delta) {
    servoAdd(1);
  } else if (t0 > t0Normal + t0Delta) {
    servoAdd(-1);
  }
  if (t0 > cfgTTailStart() && millis() - prog1StepStartTime > ((unsigned long) cfgT0MinTime()) * 60000) {
    prog1Step(6);
  }
}

// Отбор хвостов
void prog1Step6(float t0) {
  if (t0 > cfgTTailStop()) {
    cookOff();
    prog1Step(7);
  }
}

// Охлождение
void prog1Step7() {
  if (millis() - prog1StepStartTime > ((unsigned long) cfgCoolingTime()) * 60000) {
    compressorOff();
    prog1Step(8);
  }
}

// Все сделано
void prog1Step8() {
  prog1Step(255);
}


