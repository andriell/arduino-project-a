#define SD_PIN 53

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

void log(float t1, float t2, float t3, float t4) {
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile) {
    dataFile.print(timeStr());
    dataFile.print('\t');
    dataFile.print(t1);
    dataFile.print('\t');
    dataFile.print(t2);
    dataFile.print('\t');
    dataFile.print(t3);
    dataFile.print('\t');
    dataFile.println(t4);
    dataFile.close();
  }
}

