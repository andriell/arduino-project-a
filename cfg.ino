struct CfgMenuStruct {
  String vName;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};

const byte CFG_MENU_ELEMENTS_SIZE = 16;
CfgMenuStruct cfgMenuElements[] = {
  {"\222\245\254\257\245\340\240\342\343\340\240 \256\342\241\256\340\240 (C\370)", 700, 1000, 10, 1},
  {"\222\245\254\257\245\340\240\342\343\340\240 \256\342\241\256\340\240 \244\245\253\354\342\240 (C\370)", 0, 500, 10, 1},
  {"\214\250\255\250\254\240\253\354\255\256\245 \242\340\245\254\357 \256\342\241\256\340\240 \342\245\253\240 \254\250\255.", 10, 300, 1, 1},
  {"\222\245\254\257\245\340\240\342\343\340\240 \255\240\347\240\253\240 \256\342\241\256\340\240 \243\256\253\256\242 (C\370)", 0, 1000, 10, 1},
  {"\222\245\254\257\245\340\240\342\343\340\240 \255\240\347\240\253\240 \256\342\241\256\340\240 \345\242\256\341\342\256\242 (C\370)", 0, 1000, 10, 1},
  {"\222\245\254\257\245\340\240\342\343\340\240 \247\240\242\245\340\350\245\255\250\357 \256\342\241\256\340\240 \345\242\256\341\342\256\242 (C\370)", 0, 1000, 10, 1},
  {"\216\342\241\256\340 \243\256\253\256\242 \254\250\255.", 0, 100, 1, 1},
  {"\221\245\340\242\256 min \343\243\256\253", 0, 180, 1, 1},
  {"\221\245\340\242\256 max \343\243\256\253", 0, 180, 1, 1},
  {"\221\245\340\242\256 \247\240\244\245\340\246\252\240 \341\245\252.", 5, 900, 1, 1},
  {"\216\206 Max", 30, 60, 1, 1},
  {"\222\256\255 \252\255\256\257\256\252", 0, 2000, 1, 100},
  {"\210\235\215 \242\341\245\243\256 \257\256\247\250\346\250\251", 0, 100, 1, 1},
  {"\210\235\215 \257\256\247\250\346\250\357 \256\342\241\256\340\240", 1, 100, 1, 1},
  {"\210\255\342\245\340\242\240\253 \253\256\243\250\340\256\242\240\255\250\357 \341\245\252.", 10, 600, 1, 1},
  {"\202\340\245\254\357 \256\341\342\353\242\240\255\250\357 \254\250\255.", 5, 60, 1, 1},
};

float cfgT0() {
  return cfgReadFloat(0);
}

float cfgT0Delta() {
  return cfgReadFloat(1);
}

int cfgT0MinTime() {
  return cfgRead(2);
}

float cfgTHeadStart() {
  return cfgReadFloat(3);
}

float cfgTTailStart() {
  return cfgReadFloat(4);
}

float cfgTTailStop() {
  return cfgReadFloat(5);
}

int cfgTHeadTime() {
  return cfgRead(6);
}

int cfgServoMin() {
  return cfgRead(7);
}

int cfgServoMax() {
  return cfgRead(8);
}

int cfgServoTime() {
  return cfgRead(9);
}

float cfgOzMax() {
  return cfgReadFloat(10);
}

int cfgBuzzer() {
  return cfgRead(11);
}

int cfgCookStepAll() {
  return cfgRead(12);
}

int cfgCookStepGood() {
  return cfgRead(13);
}

int cfgLogTime() {
  return cfgRead(14);
}

int cfgCoolingTime() {
  return cfgRead(15);
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
