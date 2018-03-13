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

