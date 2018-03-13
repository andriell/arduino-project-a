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
  oled.print(menuList[2], 5, 10);
  
  oled.print("-1", 52, 32);
  oled.print("-10", 22, 47);
  oled.print(servoAngle, 52, 47);
  oled.print("+10", 72, 47);
  oled.print("+1", 52, 62);
  
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
