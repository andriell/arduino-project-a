void control(byte t) {
  int s;
  bool beep = false;
  for (byte i = 0; i < t; i++) {
    s = analogRead(J_X);
    if (s < 256) {
      bitSet(jButtons, 12);
      beep = true;
    } else if (s > 768) {
      bitSet(jButtons, 10);
      beep = true;
    }
    s = analogRead(J_Y);
    if (s < 256) {
      bitSet(jButtons, 13);
      beep = true;
    } else if (s > 768) {
      bitSet(jButtons, 11);
      beep = true;
    }
    if (digitalRead(J_B) == LOW) {
      bitSet(jButtons, 14);
      beep = true;
    }
    delay(100);
  }
  if (beep) {
    tone(BUZZER_PIN, 2000, 100);
  }
}
