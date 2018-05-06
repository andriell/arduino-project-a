const byte MENU_TOP = 14;
const byte MENU_SIZE = 9;
char* menuList[] = {
  "\203\340\240\344\250\252",
  "\204\240\342\240/\242\340\245\254\357",
  "\221\245\340\242\256",
  "K\240\253\250\241\340\256\242\252\240",
  "\215\240\341\342\340\256\251\252\250",
  "\220\245\252\342\250\344\250\252\240\346\250\357",
  "\204\250\341\342\250\253\253\357\346\250\357",
  "\212\256\254\257\340\245\341\341\256\340",
  "\217\245\347\354",
};
byte menuSelected = 0;
byte menuActive = 255;

void menu() {
  oledPrint("\214\245\255\356", 5, 10, 0);

  byte prev, next;
  if (menuSelected <= 0) {
    prev = MENU_SIZE - 1;
    next = menuSelected + 1;
  } else if (menuSelected >= MENU_SIZE - 1) {
    prev = menuSelected - 1;
    next = 0;
  } else {
    prev = menuSelected - 1;
    next = menuSelected + 1;
  }

  oledPrint(menuList[prev], 10, 27, 0);
  oledPrint(menuList[menuSelected], 0,  47, 1);
  oledPrint(menuList[next], 10, 59, 0);

  // Контроль
  if (bitRead(jButtons, 10)) {
    if (menuSelected <= 0) {
      menuSelected = MENU_SIZE;
    }
    menuSelected--;
  }
  if (bitRead(jButtons, 12)) {
    menuSelected++;
    if (menuSelected >= MENU_SIZE) {
      menuSelected = 0;
    }
  }
  if (bitRead(jButtons, 14)) {
    menuActive = menuSelected;
  }
}

void menuOpen(byte i) {
  menuActive = i;
}

void menuTitle(byte i) {
  oledPrint(menuList[i], 5, 10, 0);
}

void menuLoop() {
  if (menuActive == 0) {
    graphMenu();
  } else if (menuActive == 1) {
    timeMenu();
  } else if (menuActive == 2) {
    servoMenu();
  } else if (menuActive == 3) {
    calibrationMenu();
  } else if (menuActive == 4) {
    cfgMenu();
  } else if (menuActive == 5) {
    prog1Menu();
  } else if (menuActive == 6) {
    prog2Menu();
  } else if (menuActive == 7) {
    compressorMenu();
  } else if (menuActive == 8) {
    cookMenu();
  } else {
    menu();
  }
}

char nextCycleValue(byte minV, byte maxV, byte i) {
  while (i > (minV - maxV)) {
    i -= (maxV - minV) + 1;
  }
  while (i < 0) {
    i += (maxV - minV) + 1;
  }
  return minV + i;
}

