#define SERVO_TAP_PIN 5

boolean servoActive = true;
int servoPrevious = 90;
int servoAngle = 90;
unsigned long servoTime = 0;

void servoSetup() {
  lcdLog("Servo 180");
  servoAdd(180);
  lcdLog("Servo 0");
  servoAdd(-180);
  lcdLog("Servo Ok");
}

// Установка угла серво для внешних систем
// Угл устанавливается относительно текущего положения
// Если это допустимый угл, то возвращает true
boolean servoAdd(int a) {
  unsigned long waitTime = (unsigned long) cfgServoTime() * 60000;
  if (millis() - servoTime < waitTime) {
     return true;
  }
  boolean r = true;
  int vMin = cfgServoMin();
  int vMax = cfgServoMax();
  servoAngle += a;
  if (servoAngle < vMin) {
    servoAngle = vMin;
    r = false;
  }
  if (servoAngle < 0) {
    servoAngle = 0;
    r = false;
  }
  if (servoAngle > vMax) {
    servoAngle = vMax;
    r = false;
  }
  if (servoAngle > 180) {
    servoAngle = 180;
    r = false;
  }
  servoWrite(servoAngle);
  return r;
}

void servoWrite(int angle) {
  if (angle > 180 || angle < 0) {
    return;
  }
  if (servoPrevious == angle) {
    return;
  }
  servoTap.attach(SERVO_TAP_PIN);
  do {
    servoTap.write(angle);
    delay(100 + abs(angle - servoPrevious) * 10);
  } while (servoTap.read() != angle);
  servoTap.detach();
  servoPrevious = angle;
}

void servoMenu() {
  servoActive = false;
  menuTitle(2);

  oledPrint("-1", 58, 15, 0);
  oledPrint("-10", 14, 43, 0);
  oledPrintInt(servoAngle, 40, 47, 1);
  oledPrint("+10", 114, 43, 0);
  oledPrint("+1", 58, 63, 0);
  
  servoWrite(servoAngle);
  
  if (bitRead(jButtons, 10)) {
    servoAngle++;
    if (servoAngle > 180) {
      servoAngle = 180;
    }
  }
  if (bitRead(jButtons, 11)) {
    servoAngle += 10;
    if (servoAngle > 180) {
      servoAngle = 180;
    }
  }
  if (bitRead(jButtons, 12)) {
    servoAngle--;
    if (servoAngle < 0) {
      servoAngle = 0;
    }
  }
  if (bitRead(jButtons, 13)) {
    servoAngle -= 10;
    if (servoAngle < 0) {
      servoAngle = 0;
    }
  }
  if (bitRead(jButtons, 14)) {
    servoActive = true;
    menuOpen(255);
  }
}

