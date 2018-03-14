struct CfgMenuStruct {
  String vName;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};

const byte CFG_MENU_ELEMENTS_SIZE = 15;
CfgMenuStruct cfgMenuElements[] = {
  {"Температура отбора (C°)", 700, 1000, 10, 1},
  {"Температура отбора дельта (C°)", 0, 500, 10, 1},
  {"Температура начала отбора голов (C°)", 0, 100, 1, 1},
  {"Температура начала отбора хвостов (C°)", 0, 100, 1, 1},
  {"Температура завершения отбора хвостов (C°)", 0, 100, 1, 1},
  {"Отбор голов мин.", 0, 100, 1, 1},
  {"Серво min угол", 0, 180, 1, 1},
  {"Серво max угол", 0, 180, 1, 1},
  {"Серво задержка сек.", 5, 900, 1, 1},
  {"ОЖ Max", 30, 60, 1, 1},
  {"Тон кнопок", 0, 2000, 1, 100},
  {"ИЭН всего позиций", 0, 100, 1, 1},
  {"ИЭН позиция отбора", 1, 100, 1, 1},
  {"Интервал логирования сек.", 10, 600, 1, 1},
  {"Время остывания мин.", 5, 60, 1, 1},
};

float cfgT0() {
  return cfgReadFloat(0);
}

float cfgT0Delta() {
  return cfgReadFloat(1);
}

float cfgTHeadStart() {
  return cfgReadFloat(2);
}

float cfgTTailStart() {
  return cfgReadFloat(3);
}

float cfgTTailStop() {
  return cfgReadFloat(4);
}

int cfgTHeadTime() {
  return cfgRead(5);
}

int cfgServoMin() {
  return cfgRead(6);
}

int cfgServoMax() {
  return cfgRead(7);
}

int cfgServoTime() {
  return cfgRead(8);
}

float cfgOzMax() {
  return cfgReadFloat(9);
}

int cfgBuzzer() {
  return cfgRead(10);
}

int cfgCookStepAll() {
  return cfgRead(11);
}

int cfgCookStepGood() {
  return cfgRead(12);
}

int cfgLogTime() {
  return cfgRead(13);
}

int cfgCoolingTime() {
  return cfgRead(14);
}

byte cfgMenuI = 0;
void cfgMenu() {
  menuTitle(4);
  int valInt = cfgRead(cfgMenuI);
  CfgMenuStruct current = cfgMenuElements[cfgMenuI];
  float val = ((float) valInt) / current.vDivider;
  oledPrintFloat(val, 28, 30, 1);

  String vName = current.vName;
  unsigned int nameLen = vName.length();
  for (int i = 0; i <= nameLen % 21; i++) {
    oledPrint(vName.substring(i * 21, min((i + 1) * 21, nameLen)), 1, 40 + i * 10, 0);
  }
  
  // Контроль
  if (bitRead(jButtons, 10)) {
    cfgWrite(cfgMenuI, valInt + current.vStep);
  }
  if (bitRead(jButtons, 11)) {
    cfgMenuI++;
    if (cfgMenuI >= CFG_MENU_ELEMENTS_SIZE - 1) {
      cfgMenuI = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    cfgWrite(cfgMenuI, valInt - current.vStep);
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
  int val;
  EEPROM.get(addr * 2, val);
  if (val < cfgMenuElements[addr].vMin) {
    return cfgMenuElements[addr].vMin;
  }
  if (val > cfgMenuElements[addr].vMax) {
    return cfgMenuElements[addr].vMax;
  }
  return val;
}

int cfgReadFloat(byte addr) {
  return ((float) cfgRead(addr)) / cfgMenuElements[addr].vDivider;
}

// запись
void cfgWrite(byte addr, int val) {
  if (val < cfgMenuElements[addr].vMin) {
    val = cfgMenuElements[addr].vMin;
  }
  if (val > cfgMenuElements[addr].vMax) {
    val = cfgMenuElements[addr].vMax;
  }
  EEPROM.put(addr * 2, val);
}
