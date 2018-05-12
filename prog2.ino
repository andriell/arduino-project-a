#define PROG2_MAX_STEP 6

byte prog2StepI = 255;
byte prog2MenuStepI = 0;
unsigned long prog2StepStartTime = 0;
String prog2StepStartTimeStr = "";

char* prog2[] = {
  "\202\353\252\253\356\347\245\255\256",
  "\202\252\253\356\347\245\255\250\245",
  "\220\240\247\256\243\340\245\242",
  "\216\342\241\256\340",
  "\216\345\253\240\246\244\245\255\250\245",
  "\202\341\245 \341\244\245\253\240\255\256",
};

void prog2Menu() {
  menuTitle(5);
  oledPrint("\235\342\240\257", 28, 30, 1);
  oledPrintByte(prog2MenuStepI, 80, 30, 1);
  oledPrintNl(prog2[prog2MenuStepI], 40);

  // Контроль
  if (bitRead(jButtons, 10)) {
    if (prog2MenuStepI <= 0) {
      prog2MenuStepI = PROG2_MAX_STEP;
    }
    prog2MenuStepI--;
  }
  if (bitRead(jButtons, 12)) {
    prog2MenuStepI++;
    if (prog2MenuStepI >= PROG2_MAX_STEP) {
      prog2MenuStepI = 0;
    }
  }
  if (bitRead(jButtons, 14)) {
    prog2Step(prog2MenuStepI);
    prog1Step(255);
    menuOpen(255);
  }
}

void prog2Loop() {
  if (prog2StepI <= 0 || prog2StepI >= PROG2_MAX_STEP) {
    return;
  }

  lcdClear(0);
  lcdClear(1);

  lcdPrint(0, 0, prog2[prog2StepI]);

  switch (prog2StepI) {
    case 1:
      prog2Step1();
      break;
    case 2:
      prog2Step2();
      break;
    case 3:
      prog2Step3();
      break;
    case 4:
      prog2Step4();
      break;
    case 5:
      prog2Step5();
      break;
  }

  if (thermoTOZ() > cfgOzMax()) {
    lcdPrint(1, 1, "!!! \216\206 \243\256\340\357\347\240\357 !!!");
    beep(1);
  }

  if (thermoTOZ() > cfgOzMax() + 10) {
    cookOff();
    prog2Step(4);
  }

  byte x = 0;
  x = lcdPrint(x, 3, " TT ");
  x = lcdPrintFloat(x, 3, thermoTT());
  x = lcdPrint(x, 3, "  OZ ");
  x = lcdPrintFloat(x, 3, thermoTOZ());
  x = lcdPrint(x, 3, " ");
}

String prog2GetStep() {
  if (prog2StepI <= 0 || prog2StepI >= PROG2_MAX_STEP) {
    return "";
  }
  return prog2[prog2StepI];
}

void prog2Step(byte i) {
  prog2StepI = i;
  servoAdd(180); // Закрыть охлаждение
  if (i <= PROG1_MAX_STEP) {
    beep(i);
  }
  prog2StepStartTime = millis();
  prog2StepStartTimeStr = timeStr();
}

// Включение
void prog2Step1() {
  cookOn(); // Включаем плиту
  cookMax();
  prog2Step(2);
}

// Разогрев
void prog2Step2() {
  byte x = 0;
  x = lcdPrint(x, 1, "TTMIN ");
  x = lcdPrintFloat(x, 1, cfg2TTStart());
  if (thermoTT() > cfg2TTStart()) {
    compressorOn(); // Включаем компрессор
    cook2Body(); // Выставляем температуру на плите
    prog2Step(3);
  }
}

// Отбор
void prog2Step3() {
  if (thermoTT() < cfg2TTStart()) {
    lcdPrint(1, 1, "!!! TT \255\250\247\252\240\357 !!!");
    beep(1);
  }
  int ost = cfg2BodyTime() * 60 - ((millis() - prog2StepStartTime) / 1000);
  byte x = 0;
  x = lcdPrint(x, 1, "OST ");
  x = lcdPrintInt(x, 1, ost);
  if (thermoTT() > cfg2TTStop() && ost <= 0) {
    compressorOn(); // Включаем компрессор
    cookOff();
    prog2Step(4);
  }
}

// Охлождение
void prog2Step4() {
  int ost = cfg2CoolingTime() * 60 - ((millis() - prog2StepStartTime) / 1000);
  byte x = 0;
  x = lcdPrint(x, 1, "OST ");
  x = lcdPrintInt(x, 1, ost);
  if (ost <= 0) {
    compressorOff();
    prog2Step(5);
  }
}

// Все сделано
void prog2Step5() {
  beepSong1(1.2);
  prog2Step(255);
}
