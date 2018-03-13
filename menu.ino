const byte MENU_SIZE = 2;
char* menuList[] = {
  "График",
  "Дата/время",
};
byte menuSelected = 0;

void menu() {
  if (MENU_SIZE <= 5) {
    for (byte i = 0; i < MENU_SIZE; i++) {
      if (menuSelected == i) {
        oled.print(">", 5, 10 * (i + 1));
      }
      oled.print(menuList[i], 10, 10 * (i + 1));
    }
  } else {
    oled.print(">", 5, 30);
    for (byte i = 0; i < MENU_SIZE; i++) {
      byte j = nextCycleValue(0, MENU_SIZE - 1, i + menuSelected - 2);
      oled.print(menuList[j], 10, 10 * (i + 1));
    }
  }
  // Контроль
  if (bitRead(jButtons, 10)) {
    menuSelected++;
    if (menuSelected > MENU_SIZE - 1) {
      menuSelected = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    if (menuSelected <= 0) {
      menuSelected = MENU_SIZE - 1;
    } else {
      menuSelected--;
    }
  }
  if (bitRead(jButtons, 14)) {
    menuActive = menuSelected;
  }
}

char nextCycleValue(char minV, char maxV, char i) {
  while (i > (minV - maxV)) {
    i -= (maxV - minV) + 1;
  }
  while (i < 0) {
    i += (maxV - minV) + 1;
  }
  return minV + i;
}

