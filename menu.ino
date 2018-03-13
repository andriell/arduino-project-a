const byte MENU_TOP = 14;
const byte MENU_SIZE = 3;
char* menuList[] = {
  "График",
  "Дата/время",
  "Серво",
};
byte menuSelected = 0;
byte menuActive = 255;

void menu() {
  oled.print("Меню", 5, 10);
  if (MENU_SIZE <= 5) {
    for (byte i = 0; i < MENU_SIZE; i++) {
      if (menuSelected == i) {
        oled.print(">", 5, 10 * (i + 1) + MENU_TOP);
      }
      oled.print(menuList[i], 10, 10 * (i + 1) + MENU_TOP);
    }
  } else {
    oled.print(">", 5, 30 + MENU_TOP);
    for (byte i = 0; i < MENU_SIZE; i++) {
      byte j = nextCycleValue(0, MENU_SIZE - 1, i + menuSelected - 2);
      oled.print(menuList[j], 10, 10 * (i + 1) + MENU_TOP);
    }
  }
  // Контроль
  if (bitRead(jButtons, 10)) {
    if (menuSelected <= 0) {
      menuSelected = MENU_SIZE - 1;
    } else {
      menuSelected--;
    }
  }
  if (bitRead(jButtons, 12)) {
    menuSelected++;
    if (menuSelected > MENU_SIZE - 1) {
      menuSelected = 0;
    }
  }
  if (bitRead(jButtons, 14)) {
    menuActive = menuSelected;
  }
}

void menuOpen(byte i) {
  menuActive = 255;
}

void menuLoop() {
  if (menuActive == 0) {
    graphMenu();
  } else if (menuActive == 1) {
    timeMenu();
  } else if (menuActive == 2) {
    servoMenu();
  } else {
    menu();
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

