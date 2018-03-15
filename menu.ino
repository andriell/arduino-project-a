const byte MENU_TOP = 14;
const byte MENU_SIZE = 5;
char* menuList[] = {
  "\203\340\240\344\250\252",
  "\204\240\342\240/\242\340\245\254\357",
  "\221\245\340\242\256",
  "K\240\253\250\241\340\256\242\252\240",
  "\215\240\341\342\340\256\251\252\250",
};
byte menuSelected[3] = {0, 1, 2};
byte menuActive = 255;

void menu() {
  oledPrint("\214\245\255\356", 5, 10, 0);

  oledPrint(menuList[menuSelected[0]], 10, 27, 0);
  oledPrint(menuList[menuSelected[1]], 0,  47, 1);
  oledPrint(menuList[menuSelected[2]], 10, 59, 0);
  
  // Контроль
  if (bitRead(jButtons, 10)) {
    menuSelected[1]--;
  }
  if (bitRead(jButtons, 12)) {
    menuSelected[1]++;
  }
  if (bitRead(jButtons, 10) || bitRead(jButtons, 12)) {
    menuSelected[0] = nextCycleValue(0, MENU_SIZE, menuSelected[1] - 1);
    menuSelected[1] = nextCycleValue(0, MENU_SIZE, menuSelected[1]);
    menuSelected[2] = nextCycleValue(0, MENU_SIZE, menuSelected[1] + 1);
  }
  if (bitRead(jButtons, 14)) {
    menuActive = menuSelected[1];
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

