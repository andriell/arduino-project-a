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
  sprintf(logFileName, "%04d%02d%02d.csv", now.year(), now.month(), now.day());
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

void logLoop() {
  unsigned long m = millis();
  if (m - logTime < 10000) {
    return;
  }
  logTime = m;
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile) {
    dataFile.print(timeStr());
    dataFile.print('\t');
    dataFile.print(thermoT0());
    dataFile.print('\t');
    dataFile.print(thermoTC());
    dataFile.print('\t');
    dataFile.print(thermoTT());
    dataFile.print('\t');
    dataFile.print(thermoTOZ());
    dataFile.print('\t');
    dataFile.print(servoGetAngle());
    dataFile.print('\t');
    dataFile.println(prog1GetStep());
    dataFile.close();
  }
}

