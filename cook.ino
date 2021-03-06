#define COOK_P_PIN 33
#define COOK_O_PIN 34
#define COOK_M_PIN 35

unsigned long cookTime = 0UL;

void cookSetup() {
  pinMode(COOK_P_PIN, OUTPUT);
  pinMode(COOK_O_PIN, OUTPUT);
  pinMode(COOK_M_PIN, OUTPUT);

  digitalWrite(COOK_P_PIN, HIGH);
  digitalWrite(COOK_O_PIN, HIGH);
  digitalWrite(COOK_M_PIN, HIGH);
}

void cookLoop() {
  if (cookTime > 10UL && millis() - cookTime > 1800000UL) {
    Serial.println("Cook restart");
    cookTime = millis();
    cookP(1);
    cookM(1);
  }
}

void cookMenu() {
  menuTitle(7);
  oledPrint("\217\250\342\240\255\250\245", 40, 30, 0);
  oledPrint("-     +", 40, 50, 0);
  if (bitRead(jButtons, 10)) {
    cookOn();
  }
  if (bitRead(jButtons, 11)) {
    cookP(1);
  }
  if (bitRead(jButtons, 13)) {
    cookM(1);
  }
  if (bitRead(jButtons, 14)) {
    menuOpen(255);
  }
}

void cookOn() {
  digitalWrite(COOK_O_PIN, LOW);
  delay(200);
  digitalWrite(COOK_O_PIN, HIGH);
  cookTime = millis();
}

void cookP(int n) {
  for (int i = 0; i < n; i++) {
    delay(200);
    digitalWrite(COOK_P_PIN, LOW);
    delay(200);
    digitalWrite(COOK_P_PIN, HIGH);
  }
}

void cookM(int n) {
  for (int i = 0; i < n; i++) {
    delay(200);
    digitalWrite(COOK_M_PIN, LOW);
    delay(200);
    digitalWrite(COOK_M_PIN, HIGH);
  }
}

void cookMax() {
  int n = cfgCookStepAll();
  cookP(n);
}

void cookMin() {
  int n = cfgCookStepAll();
  cookM(n);
}

void cookHead() {
  int n = cfgCookStepAll();
  cookM(n);
  n = cfgCookStepHead();
  cookP(n);
}

void cookBody() {
  int n = cfgCookStepAll();
  cookM(n);
  n = cfgCookStepBody();
  cookP(n);
}

void cookTail() {
  int n = cfgCookStepAll();
  cookM(n);
  n = cfgCookStepTail();
  cookP(n);
}

void cook2Body() {
  int n = cfgCookStepAll();
  cookM(n);
  n = cfg2CookBody();
  cookP(n);
}

void cookOff() {
  digitalWrite(COOK_O_PIN, LOW);
  delay(200);
  digitalWrite(COOK_O_PIN, HIGH);
  cookTime = 0;
}

