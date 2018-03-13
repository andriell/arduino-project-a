byte lcdLogI = 0;

void lcdSetup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcdLog("LCD Loaded");
}

void lcdLog(String str) {
  Serial.println(str);
  lcdLogI++;
  if (lcdLogI >= 4) {
    lcdLogI = 0;
  }
  lcd.setCursor(0, lcdLogI);
  lcd.print("                    ");
  lcd.setCursor(0, lcdLogI);
  lcd.print(str);
  delay(1000);
}

void lcdTemperature(float* t, byte l) {
  for (byte i = 0; i < l; i++) {
    lcd.setCursor(0, i);
    lcd.print("     ");
    lcd.setCursor(0, i);
    lcd.print(t[i]);
  }
}

