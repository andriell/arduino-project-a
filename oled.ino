extern uint8_t SmallFontRus[];

void oledSetap() {
  lcdLog("OLED loading...");
  oled.begin();           // Инициируем работу с дисплеем.
  oled.setFont(SmallFontRus);
  oled.autoUpdate(false); // Запрещаем автоматический вывод данных. Информация на дисплее будет обновляться только после обращения к функции update().
  oled.clrScr();
  oled.print("OLED", 0, 7);
  oled.update();
  lcdLog("OLEDed loaded");
}

