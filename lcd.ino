byte lcdLogI = 0;

void lcdSetup() {
  lcd.init();
  mytkoLcdCreateChar(lcd);
  lcd.backlight();
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcdLog("LCD Loaded");
}

void lcdLog(String str) {
  str = mutko(str);
  Serial.println(str);
  lcdClear(lcdLogI);
  lcdPrint(0, lcdLogI, str);
  lcdLogI++;
  if (lcdLogI > 3) {
    lcdLogI = 0;
  }
  delay(200);
}

byte lcdPrint(byte x, byte y, String str) {
  lcd.setCursor(x, y);
  lcd.print(mutko(str));
  return x + str.length();
}

byte lcdPrintInt(byte x, byte y, int str) {
  lcd.setCursor(x, y);
  lcd.print(str);
  if (str < 0) {
    x++;
    str = abs(str);
  }
  do {
    x++;
    str = str / 10;
  } while (str > 0);
  return x;
}

byte lcdPrintFloat(byte x, byte y, float str) {
  lcd.setCursor(x, y);
  lcd.print(str);
  return x + 5;
}

void lcdClear(byte y) {
  lcd.setCursor(0, y);
  lcd.print("                    ");
}

