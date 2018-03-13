byte logI = 0;

void lcdLog(String str) {
  Serial.println(str);
  logI++;
  if (logI >= 4) {
    logI = 0;
  }
  lcd.setCursor(0, logI);
  lcd.print("                    ");
  lcd.setCursor(0, logI);
  lcd.print(str);
  delay(1000);
}

void lcdTemperature(byte i, float t) {
  lcd.setCursor(0, i);
  lcd.print("     ");
  lcd.setCursor(0, i);
  lcd.print(t);
}

