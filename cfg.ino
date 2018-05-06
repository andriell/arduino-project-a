struct CfgMenuStruct {
  byte vAddr;
  String vName;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};
const byte CFG_MENU_ELEMENTS_SIZE = 20;
CfgMenuStruct cfgMenuElements[] = {
  {0, "\220. \222\245\254\257\245\340\240\342\343\340\240 \256\342\241\256\340\240 (C\370)", 700, 1000, 10, 1},
  {1, "\220. \222\245\254\257\245\340\240\342\343\340\240 \256\342\241\256\340\240 \244\245\253\354\342\240 (C\370)", 0, 500, 10, 1},
  {2, "\220. \214\250\255\250\254\240\253\354\255\256\245 \242\340\245\254\357 \256\342\241\256\340\240 \342\245\253\240 \254\250\255.", 10, 300, 1, 1},
  {3, "\220. \222\245\254\257\245\340\240\342\343\340\240 \255\240\347\240\253\240 \256\342\241\256\340\240 \243\256\253\256\242 (C\370)", 0, 100, 1, 1},
  {4, "\220. \222\245\254\257\245\340\240\342\343\340\240 \255\240\347\240\253\240 \256\342\241\256\340\240 \345\242\256\341\342\256\242 (C\370)", 0, 100, 1, 1},
  {5, "\220. \222\245\254\257\245\340\240\342\343\340\240 \247\240\242\245\340\350\245\255\250\357 \256\342\241\256\340\240 \345\242\256\341\342\256\242 (C\370)", 0, 100, 1, 1},
  {6, "\220. \216\342\241\256\340 \243\256\253\256\242 \254\250\255.", 0, 100, 1, 1},
  {13, "\220. \210\235\215 \257\256\247\250\346\250\357 \256\342\241\256\340\240 \243\256\253\256\242", 0, 20, 1, 1},
  {16, "\220. \210\235\215 \257\256\247\250\346\250\357 \256\342\241\256\340\240 \342\245\253\240", 0, 20, 1, 1},
  {17, "\220. \210\235\215 \257\256\247\250\346\250\357 \256\342\241\256\340\240 \345\242\256\341\342\256\242", 0, 20, 1, 1},
  {15, "\220. \202\340\245\254\357 \256\341\342\353\242\240\255\250\357 \254\250\255.", 5, 60, 1, 1},
  {7, "\204. \222\245\254\257\245\340\240\342\343\340\240 \255\240\347\240\253\240 \256\342\241\256\340\240", 400, 1000, 10, 1},
  {8, "\204. \222\245\254\257\245\340\240\342\343\340\240 \247\240\242\245\340\350\245\255\250\357 \256\342\241\256\340\240", 400, 1000, 10, 1},
  {9, "\204. \214\250\255\250\254\240\253\354\255\256\245 \242\340\245\254\357 \256\342\241\256\340\240 \254\250\255.", 10, 300, 1, 1},
  {18, "\204. \210\235\215 \257\256\247\250\346\250\357 \256\342\241\256\340\240", 0, 20, 1, 1},
  {19, "\204. \202\340\245\254\357 \256\341\342\353\242\240\255\250\357 \254\250\255.", 5, 60, 1, 1},
  {10, "\220\204. \216\206 Max", 30, 60, 1, 1},
  {12, "\220\204. \210\235\215 \242\341\245\243\256 \257\256\247\250\346\250\251", 0, 20, 1, 1},
  {14, "\210\255\342\245\340\242\240\253 \253\256\243\250\340\256\242\240\255\250\357 \341\245\252.", 10, 600, 1, 1},
  {11, "\222\256\255 \252\255\256\257\256\252", 0, 2000, 1, 100},
  //  {7, "\220. \221\245\340\242\256 min \343\243\256\253", 0, 180, 1, 1},
  //  {8, "\220. \221\245\340\242\256 max \343\243\256\253", 0, 180, 1, 1},
  //  {9, "\220. \221\245\340\242\256 \247\240\244\245\340\246\252\240 \341\245\252.", 5, 900, 1, 1},
};

byte cfgAddr2I[CFG_MENU_ELEMENTS_SIZE];

void cfgSetup() {
  for (byte i = 0; i < CFG_MENU_ELEMENTS_SIZE; i++) {
    cfgAddr2I[cfgMenuElements[i].vAddr] = i;
    
  }
  for (byte i = 0; i < 20; i++) {
    Serial.print(i);
    Serial.print(" ");
    Serial.println(cfgAddr2I[i]);
  }
}

byte cfgMenuI = 0;
void cfgMenu() {
  menuTitle(4);
  CfgMenuStruct current = cfgMenuElements[cfgMenuI];
  int valInt = cfgRead(current.vAddr);
  float val = ((float) valInt) / current.vDivider;
  oledPrintFloat(val, 28, 30, 1);

  oledPrintNl(current.vName, 40);

  // Контроль
  if (bitRead(jButtons, 10)) {
    cfgWrite(current.vAddr, valInt + current.vStep);
  }
  if (bitRead(jButtons, 11)) {
    cfgMenuI++;
    if (cfgMenuI >= CFG_MENU_ELEMENTS_SIZE - 1) {
      cfgMenuI = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    cfgWrite(current.vAddr, valInt - current.vStep);
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
  if (val < cfgMenuElements[cfgAddr2I[addr]].vMin) {
    return cfgMenuElements[cfgAddr2I[addr]].vMin;
  }
  if (val > cfgMenuElements[cfgAddr2I[addr]].vMax) {
    return cfgMenuElements[cfgAddr2I[addr]].vMax;
  }
  return val;
}

float cfgReadFloat(byte addr) {
  return ((float) cfgRead(addr)) / cfgMenuElements[cfgAddr2I[cfgMenuI]].vDivider;
}

// запись
void cfgWrite(byte addr, int val) {
  if (val < cfgMenuElements[cfgAddr2I[addr]].vMin) {
    val = cfgMenuElements[cfgAddr2I[addr]].vMin;
  }
  if (val > cfgMenuElements[cfgAddr2I[addr]].vMax) {
    val = cfgMenuElements[cfgAddr2I[addr]].vMax;
  }
  EEPROM.put(addr * 2, val);
}

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
  return 10;
}

int cfgServoMax() {
  return 170;
}

int cfgServoTime() {
  return 10;
}

float cfg2TTStart() {
  return cfgReadFloat(7);
}

float cfg2TTStop() {
  return cfgReadFloat(8);
}

int cfg2BodyTime() {
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

int cfgCookStepHead() {
  return cfgRead(13);
}

int cfgLogTime() {
  return cfgRead(14);
}

int cfgCoolingTime() {
  return cfgRead(15);
}

int cfgCookStepBody() {
  return cfgRead(16);
}

int cfgCookStepTail() {
  return cfgRead(17);
}

int cfg2CookBody() {
  return cfgRead(18);
}

int cfg2CoolingTime() {
  return cfgRead(19);
}
