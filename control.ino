const int J_X_PIN = A1;
const int J_Y_PIN = A0;
const int J_B_PIN = 2;
// 0 - 9 - цифры
// 10 - вверх
// 11 - вправо
// 12 - вниз
// 13 - влево
// 14 - Ok
// 15 - cancel

const int BUZZER_PIN = 3;

void controlSetup() {
  pinMode(J_X_PIN, INPUT);
  pinMode(J_Y_PIN, INPUT);
  pinMode(J_B_PIN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 2000, 100);
}

void control(byte t) {
  int s;
  bool beep = false;
  for (byte i = 0; i < t; i++) {
    if (i > 0) {
      delay(100);
    }
    s = analogRead(J_X_PIN);
    if (s < 256) {
      bitSet(jButtons, 12);
      beep = true;
      break;
    } else if (s > 768) {
      bitSet(jButtons, 10);
      beep = true;
      break;
    }
    s = analogRead(J_Y_PIN);
    if (s < 256) {
      bitSet(jButtons, 13);
      beep = true;
      break;
    } else if (s > 768) {
      bitSet(jButtons, 11);
      beep = true;
      break;
    }
    if (digitalRead(J_B_PIN) == LOW) {
      bitSet(jButtons, 14);
      beep = true;
      break;
    }
  }
  if (beep) {
    int vTone = cfgBuzzer();
    if (vTone > 0) {
      tone(BUZZER_PIN, vTone, 100);
    }
  }
}

void controlBeep(byte n) {
  for (byte i = 0; i < n; i++) {
    tone(BUZZER_PIN, 2000, 100);
    delay(200);
  }
}

