char* prog1[] = {
  "\202\252\253\356\347\245\255\250\245",
  "\220\240\247\256\243\340\245\242",
  "\216\342\241\256\340 \243\256\253\256\242",
  "\216\342\241\256\340 \342\245\253\240 1",
  "\216\342\241\256\340 \342\245\253\240 2",
  "\216\345\253\256\246\244\245\255\250\245",
  "\202\341\245 \341\244\245\253\240\255\256",
};
byte prog1Step = 0;
String prog1StepStartTime = "";
unsigned long prog1HeadStartTime = 0;

void prog1Loop(float t0, float tTank, float tCooler, float tOz) {
  float t0Normal = cfgT0();
  float t0Delta = cfgT0Delta();

  if (tOz > cfgOzMax()) {
    controlBeep(10);
  }

  if (tOz > cfgOzMax() + 10) {
    prog1Step = 5;
  }

  if (prog1Step == 0) {
    // Включение
    cookOn(); // Включаем плиту
    servoAdd(180); // Закрыть охлаждение
    prog1Step = 1;
    controlBeep(1);
    prog1StepStartTime = timeStr();

  } else if (prog1Step == 1) {
    // Разогрев
    float tHeadStart = cfgTHeadStart(); // Начальная температура отбора голов
    servoAdd(180); // Закрыть охлаждение
    if (t0 > tHeadStart) {
      compressorOn(); // Включаем компрессор
      cookGood(); // Выставляем температуру на плите
      prog1HeadStartTime = millis();
      prog1Step = 2;
      controlBeep(2);
      prog1StepStartTime = timeStr();
    }

  } else if (prog1Step == 2) {
    // Отбор голов
    unsigned long headTime = (unsigned long) cfgTTailStart() * 60000;
    servoAdd(180); // Закрыть охлаждение
    if (millis() - prog1HeadStartTime > headTime) {
      prog1Step = 3;
      controlBeep(3);
      prog1StepStartTime = timeStr();
    }

  } else if (prog1Step == 3) {
    // Отбор тела 1
    servoAdd(180); // Закрыть охлаждение
    if (t0 > t0Normal) {
      prog1Step = 4;
      controlBeep(4);
      prog1StepStartTime = timeStr();
    }

  } else if (prog1Step == 4) {
    // Отбор тела 2
    if (t0 < t0Normal - t0Delta) {
      servoAdd(1);
    } else if (t0 > t0Normal + t0Delta) {
      servoAdd(-1);
    }
    if (t0 > t0Normal) {
      prog1Step = 4;
      controlBeep(4);
      prog1StepStartTime = timeStr();
    }

  } else if (prog1Step == 5) {
    // Охлождение


  }
}

