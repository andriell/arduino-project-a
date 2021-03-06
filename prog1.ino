#define PROG1_MAX_STEP 10

byte prog1StepI = 255;
byte prog1MenuStepI = 0;
unsigned long prog1StepStartTime = 0;
String prog1StepStartTimeStr = "";



char* prog1[] = {
  "\202\353\252\253\356\347\245\255\256",
  "\202\252\253\356\347\245\255\250\245",
  "\220\240\247\256\243\340\245\242",
  "\216\342\241\256\340 \243\256\253\256\242",
  "\215\240\347\240\253\256 \342\245\253\240",
  "\216\342\241\256\340 \342\245\253\240",
  "\215\240\347\240\253\256 \345\242\256\341\342\256\242",
  "\216\342\241\256\340 \345\242\256\341\342\256\242",
  "\216\345\253\240\246\244\245\255\250\245",
  "\202\341\245 \341\244\245\253\240\255\256",
};

void prog1Menu() {
  menuTitle(5);
  oledPrint("\235\342\240\257", 28, 30, 1);
  oledPrintByte(prog1MenuStepI, 80, 30, 1);
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
    if (prog1MenuStepI >= PROG1_MAX_STEP) {
      prog1MenuStepI = 0;
    }
  }
  if (bitRead(jButtons, 14)) {
    prog1Step(prog1MenuStepI);
    prog2Step(255);
    menuOpen(255);
  }
}

void prog1Loop() {
  if (prog1StepI <= 0 || prog1StepI >= PROG1_MAX_STEP) {
    return;
  }

  lcdClear(0);
  lcdClear(1);

  lcdPrint(0, 0, prog1[prog1StepI]);

  switch (prog1StepI) {
    case 1:
      prog1Step1();
      break;
    case 2:
      prog1Step2();
      break;
    case 3:
      prog1Step3();
      break;
    case 4:
      prog1Step4();
      break;
    case 5:
      prog1Step5();
      break;
    case 6:
      prog1Step6();
      break;
    case 7:
      prog1Step7();
      break;
    case 8:
      prog1Step8();
      break;
    case 9:
      prog1Step9();
      break;
  }

  if (thermoTOZ() > cfgOzMax()) {
    lcdPrint(1, 1, "!!! \216\206 \243\256\340\357\347\240\357 !!!");
    beep(1);
  }

  if (thermoTOZ() > cfgOzMax() + 10) {
    cookOff();
    prog1Step(7);
  }

  byte x = 0;
  x = lcdPrint(x, 2, " TO ");
  x = lcdPrintFloat(x, 2, thermoT0());
  x = lcdPrint(x, 2, "  TC ");
  x = lcdPrintFloat(x, 2, thermoTC());
  x = lcdPrint(x, 2, " ");

  x = 0;
  x = lcdPrint(x, 3, " TT ");
  x = lcdPrintFloat(x, 3, thermoTT());
  x = lcdPrint(x, 3, "  OZ ");
  x = lcdPrintFloat(x, 3, thermoTOZ());
  x = lcdPrint(x, 3, " ");
}

void prog1ControlT0() {
  if (thermoT0() < cfgT0() - cfgT0Delta()) {
    lcdPrint(1, 1, "!!! T0 \255\250\247\252\240\357 !!!");
    beep(1);
  }
}

String prog1GetStep() {
  if (prog1StepI <= 0 || prog1StepI >= PROG1_MAX_STEP) {
    return "";
  }
  return prog1[prog1StepI];
}

void prog1Step(byte i) {
  prog1StepI = i;
  if (i <= PROG1_MAX_STEP) {
    beep(i);
  }
  prog1StepStartTime = millis();
  prog1StepStartTimeStr = timeStr();
}

// Включение
void prog1Step1() {
  cookOn(); // Включаем плиту
  cookMax();
  prog1Step(2);
}

// Разогрев
void prog1Step2() {
  byte x = 0;
  x = lcdPrint(x, 1, "T0MIN ");
  x = lcdPrintInt(x, 1, (int) cfgTHeadStart());
  if (thermoT0() > cfgTHeadStart()) {
    compressorOn(); // Включаем компрессор
    cookHead(); // Выставляем температуру на плите
    prog1Step(3);
  }
}

// Отбор голов
void prog1Step3() {
  int ost = cfgTHeadTime() * 60 - ((millis() - prog1StepStartTime) / 1000);
  byte x = 0;
  x = lcdPrint(x, 1, "OST ");
  x = lcdPrintInt(x, 1, ost);
  prog1ControlT0();
  if (ost <= 0) {
    compressorOn(); // Включаем компрессор
    cookMin();
    prog1Step(4);
  }
}

// Начало тела
void prog1Step4() {
  lcdPrint(1, 1, "\215\240\347\240\253\256 \342\245\253\240");
  beep(2);
  if (bitRead(jButtons, 14)) {
    compressorOn(); // Включаем компрессор
    cookBody();  // Выставляем температуру на плите
    prog1Step(5);
  }
}

// Отбор тела
void prog1Step5() {
  float t0Normal = cfgT0();
  byte x = 0;
  x = lcdPrint(x, 1, "T0N ");
  x = lcdPrintFloat(x, 1, t0Normal);
  prog1ControlT0();
  int ost = cfgT0MinTime() * 60 - ((millis() - prog1StepStartTime) / 1000);
  if (thermoT0() > cfgTTailStart() && ost <= 0) {
    compressorOn();  // Включаем компрессор
    cookMin();
    prog1Step(6);
  }
}

// Начало хвостов
void prog1Step6() {
  lcdPrint(1, 1, "\215\240\347\240\253\256 \345\242\256\341\342\256\242");
  beep(2);
  if (bitRead(jButtons, 14)) {
    compressorOn(); // Включаем компрессор
    cookTail();
    prog1Step(7);
  }
}

// Отбор хвостов
void prog1Step7() {
  float tTailStop = cfgTTailStop();
  byte x = 0;
  x = lcdPrint(x, 1, "T0MAX ");
  x = lcdPrintFloat(x, 1, tTailStop);
  prog1ControlT0();
  if (thermoT0() > tTailStop) {
    cookOff();
    prog1Step(8);
  }
}

// Охлождение
void prog1Step8() {
  int ost = cfgCoolingTime() * 60 - ((millis() - prog1StepStartTime) / 1000);
  byte x = 0;
  x = lcdPrint(x, 1, "OST ");
  x = lcdPrintInt(x, 1, ost);
  if (ost <= 0) {
    compressorOff();
    prog1Step(9);
  }
}

// Все сделано
void prog1Step9() {
  beepSong1(1);
  prog1Step(255);
}
