#define COMPESSOR_PIN 32

void compressorSetup() {
  pinMode(COMPESSOR_PIN, OUTPUT);
  digitalWrite(COMPESSOR_PIN, HIGH);
}

void compressorMenu() {
  menuTitle(6);
  oledPrint("\204\240", 40, 30, 1);
  oledPrint("\215\245\342", 40, 50, 1);
  oledPrint(">", 28, digitalRead(COMPESSOR_PIN) == LOW ? 30 : 50, 1);

  if (bitRead(jButtons, 10) || bitRead(jButtons, 12)) {
    if (digitalRead(COMPESSOR_PIN) == HIGH) {
      digitalWrite(COMPESSOR_PIN, LOW);
    } else {
      digitalWrite(COMPESSOR_PIN, HIGH);
    }
  }

  if (bitRead(jButtons, 14)) {
    menuOpen(255);
  }
}

void compressorOn() {
  digitalWrite(COMPESSOR_PIN, LOW);
}

void compressorOff() {
  digitalWrite(COMPESSOR_PIN, HIGH);
}

