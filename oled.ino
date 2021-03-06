iarduino_OLED oled(0x3C);
// ширина символов (6), высота символов (8),
extern uint8_t SmallFontRus[];
// ширина символов (12), высота символов (16)
extern uint8_t MediumFontRus[];
byte oledFSPrev = 255;

void oledSetap() {
  lcdLog("OLED loading...");
  oled.begin();
  oledClean();
  oled.autoUpdate(false);
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
  oled.update();
}
void oledPixel(int x, int y) {
  oled.drawPixel(x, y, true);
}

void oledPrint(String txt, int x, int y, byte fs) {
  oledFont(fs);
  oled.print(txt, x, y);
}

byte oledPrintNl(String txt, int y) {
  unsigned int len = txt.length();
  for (int i = 0; i <= len % 21; i++) {
    oledPrint(txt.substring(i * 21, min((i + 1) * 21, len)), 1, y + i * 10, 0);
  }
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


