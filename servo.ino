#define SERVO_TAP_PIN 5

boolean servoActive = true;
int servoPrevious = 90;
int servoAngle = 90;

void servoSetup() {
  lcdLog("Servo 180");
  servoWrite(180);
  lcdLog("Servo 0");
  servoWrite(0);
  lcdLog("Servo Ok");
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
