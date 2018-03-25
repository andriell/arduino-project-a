#define CALIBRATION_FILE "calib.csv"
boolean calibrationActive = false;
int calibrationT0Prev = 0;

void calibrationMenu() {
  menuTitle(3);
  oledPrint("Да", 40, 30, 1);
  oledPrint("Нет", 40, 50, 1);
  oledPrint(">", 28, calibrationActive ? 30 : 50, 1);
  if (bitRead(jButtons, 10)) {
    calibrationActive = !calibrationActive;
  }
  if (bitRead(jButtons, 12)) {
    calibrationActive = !calibrationActive;
  }
  if (bitRead(jButtons, 14)) {
    if (calibrationActive && SD.exists(CALIBRATION_FILE)) {
      SD.remove(CALIBRATION_FILE);
    }
    menuOpen(255);
  }
}

void calibrationLoop(float* t, byte lenght) {
  if (!calibrationActive) {
    return;
  }
  int t0 = thermoT0Int();
  if (calibrationT0Prev == t0) {
    return;
  }
  File dataFile = SD.open(CALIBRATION_FILE, FILE_WRITE);
  if (dataFile) {
    dataFile.print(timeStr());
    dataFile.print('\t');
    dataFile.print(t0);
    dataFile.print('\t');
    for (byte i = 0; i < lenght; i++) {
      dataFile.print('\t');
      dataFile.print(t[i]);
    }
    dataFile.println();
    dataFile.close();
  }
  calibrationT0Prev = t0;
}

