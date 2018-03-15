iarduino_OLED oled(0x3C);
// ширина символов (6), высота символов (8),
extern uint8_t SmallFontRus[];
// ширина символов (12), высота символов (16)
extern uint8_t MediumFontRus[];
byte oledFSPrev = 0;

void oledSetap() {
  lcdLog("OLED loading...");
  oled.begin();
  oled.setCoding(false);
  oled.autoUpdate(false);
  oledClean();
  oledPrint("OLED", 40, 40, 1);
  oledUpdate();
  lcdLog("OLEDed loaded");
}

void oledFont(byte fs) {
  if (fs == oledFSPrev) {
    return;
  }
  if (fs == 0) {
    oled.setFont(SmallFontRus);
  } else if (fs == 1) {
    oled.setFont(MediumFontRus);
  }
  fs = oledFSPrev;
}

void oledClean() {
  oled.clrScr();
}

void oledUpdate() {
  oled.clrScr();
}
void oledPixel(int x, int y) {
  oled.drawPixel(x, y, true);
}

void oledPrint(String txt, int x, int y, byte fs) {
  oledFont(fs);
  oled.print(txt, x, y);
}

void oledPrintByte(byte txt, int x, int y, byte fs) {
  oledFont(fs);
  oled.print(txt, x, y);
}

void oledPrintInt(int txt, int x, int y, byte fs) {
  oledFont(fs);
  oled.print(txt, x, y);
}

void oledPrintFloat(float txt, int x, int y, byte fs) {
  oledFont(fs);
  oled.print(txt, x, y);
}


