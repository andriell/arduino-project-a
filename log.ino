#define SD_PIN 53
unsigned long logTime = 0;

void logSetup() {
  lcdLog("SD loading...");
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_PIN)) {
    lcdLog("SD Error");
    while (1);
  }
  DateTime now = rtc.now();
  sprintf(logFileName, "%04d%02d%02d.txt", now.year(), now.month(), now.day());
  logFileName[12] = '\0';
  lcdLog(logFileName);
  File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) {
    logFile.close();
  } else {
    lcdLog("File open Error");
    while (1);
  }
  lcdLog("SD loaded");
}

void logThermo(float* t, byte lenght) {
  unsigned long m = millis();
  if (m - logTime < 10000) {
    return;
  }
  logTime = m;
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile) {
    dataFile.print(timeStr());
    for (byte i = 0; i < lenght; i++) {
      dataFile.print('\t');
      dataFile.print(t[i]);
    }
    dataFile.println();
    dataFile.close();
  }
}

