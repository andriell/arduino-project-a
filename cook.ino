#define COOK_O_PIN 34
#define COOK_M_PIN 35

void cookSetup() {
  pinMode(COOK_O_PIN, OUTPUT);
  pinMode(COOK_M_PIN, OUTPUT);

  digitalWrite(COOK_O_PIN, HIGH);
  digitalWrite(COOK_M_PIN, HIGH);
}

void cookMenu() {
  menuTitle(7);
  oledPrint("\217\250\342\240\255\250\245", 40, 30, 0);
  oledPrint("-", 40, 50, 0);
  if (bitRead(jButtons, 10)) {
    cookOn();
  }
  if (bitRead(jButtons, 12)) {
    digitalWrite(COOK_M_PIN, LOW);
    delay(200);
    digitalWrite(COOK_M_PIN, HIGH);
  }
  if (bitRead(jButtons, 14)) {
    menuOpen(255);
  }
}

void cookOn() {
  digitalWrite(COOK_O_PIN, LOW);
  delay(200);
  digitalWrite(COOK_O_PIN, HIGH);
}

void cookGood() {
  int n = cfgCookStepGood();
  for (int i = 0; i < n; i++) {
    digitalWrite(COOK_M_PIN, LOW);
    delay(200);
    digitalWrite(COOK_M_PIN, HIGH);
  }
}

void cookOff() {
  digitalWrite(COOK_O_PIN, LOW);
  delay(200);
  digitalWrite(COOK_O_PIN, HIGH);
}

