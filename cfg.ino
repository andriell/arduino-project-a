const byte CFG_MENU_ELEMENTS_SIZE = 11;

struct CfgMenuStruct {
  String vName;
  int vMax;
  int vMin;
  int vDivider;
  int vStep;
};
CfgMenuStruct cfgMenuElements[] = {
  {"Температура отбора (C°)", 70, 100, 10, 1},
  {"Температура отбора дельта (C°)", 0, 50, 10, 1},
  {"Серво задержка сек.", 5, 900, 1, 1},
  {"Серво max °", 0, 180, 1, 1},
  {"Серво min °", 0, 180, 1, 1},
  {"ОЖ Max", 30, 60, 1, 1},
  {"Отбор голов мин.", 0, 100, 1, 1},
  {"Тон пищалки", 100, 20000, 1, 100},
  {"ИЭН всего позиций", 0, 100, 1, 1},
  {"ИЭН позиция отбора", 1, 100, 1, 1},
  {"Время остывания мин.", 5, 60, 1, 1},
};

byte cfgMenuI = 0;

void cfgMenu() {
  int valInt = cfgRead(cfgMenuI);
  float val = ((float) valInt) / cfgMenuElements[cfgMenuI].vDivider;
  oledPrintFloat(val, 28, 30, 1);
  oledPrint(cfgMenuElements[cfgMenuI].vName, 2, 40, 0);

  // Контроль
  if (bitRead(jButtons, 10)) {
    valInt += cfgMenuElements[cfgMenuI].vStep;
    if (valInt > cfgMenuElements[cfgMenuI].vMax) {
      valInt = cfgMenuElements[cfgMenuI].vMax;
    }
    cfgWrite(cfgMenuI, valInt);
  }
  if (bitRead(jButtons, 11)) {
    cfgMenuI++;
    if (cfgMenuI > CFG_MENU_ELEMENTS_SIZE) {
      cfgMenuI = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    valInt -= cfgMenuElements[cfgMenuI].vStep;
    if (valInt < cfgMenuElements[cfgMenuI].vMin) {
      valInt = cfgMenuElements[cfgMenuI].vMin;
    }
    cfgWrite(cfgMenuI, valInt);
  }
  if (bitRead(jButtons, 13)) {
    if (cfgMenuI <= 0) {
      cfgMenuI = CFG_MENU_ELEMENTS_SIZE;
    }
    cfgMenuI--;
  }
  if (bitRead(jButtons, 14)) {
    menuOpen(255);
  }
}

// чтение
int cfgRead(byte addr) {
  addr *= 2;
  byte raw[2];
  raw[0] = EEPROM.read(addr);
  raw[1] = EEPROM.read(addr + 1);
  int &num = (int&) raw;
  return num;
}

// запись
void cfgWrite(byte addr, int num) {
  addr *= 2;
  byte raw[2];
  (int&) raw = num;
  EEPROM.write(addr, raw[0]);
  EEPROM.write(addr + 1, raw[1]);
}
