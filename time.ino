int timeNew[6] = {2018, 6, 15, 12, 30, 0};
byte timeNewI = 0;

void timeSetup() {
  lcdLog("RTC loading...");
  if (!rtc.begin()) {
    lcdLog("RTC Error");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(2018, 1, 1, 0, 0, 0));
    menuOpen(1);
  }
  lcdLog(timeStr());
  lcdLog("RTC loaded");
}

void timeMenu() {
  menuTitle(1);
  oledPrint(timeStr(), 5, 24, 0);

  char timeNewStr[20];
  sprintf(timeNewStr, "%04d-%02d-%02d %02d:%02d:%02d", timeNew[0], timeNew[1], timeNew[2], timeNew[3], timeNew[4], timeNew[5]);
  timeNewStr[19] = '\0';

  oledPrint(timeNewStr, 5, 34, 0);

  char* timeNewName[] = {
    "\203\256\244",
    "\214\245\341\357\346",
    "\204\245\255\354",
    "\227\240\341",
    "\214\250\255\343\342\240",
    "\221\245\252\343\255\244\240",
    "\221\256\345\340\240\255\250\342\354",
    "\202\353\345\256\244",
  };
  oledPrint(timeNewName[timeNewI], 5, 44, 0);

  if (bitRead(jButtons, 10)) {
    if (timeNewI < 6) {
      timeNew[timeNewI]++;
      if (timeNewI == 1 && timeNew[timeNewI] > 12) {
        timeNew[timeNewI] = 1;
      } else if (timeNewI == 2 && timeNew[timeNewI] > 31) {
        timeNew[timeNewI] = 1;
      } else if (timeNewI == 3 && timeNew[timeNewI] > 23) {
        timeNew[timeNewI] = 0;
      } else if ((timeNewI == 4 || timeNewI == 5) && timeNew[timeNewI] > 59) {
        timeNew[timeNewI] = 0;
      }
    }
  }
  if (bitRead(jButtons, 11)) {
    timeNewI++;
    if (timeNewI > 7) {
      timeNewI = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    if (timeNewI < 6) {
      timeNew[timeNewI]--;
      if (timeNewI == 0 && timeNew[timeNewI] < 2000) {
        timeNew[timeNewI] = 2000;
      } else if (timeNewI == 1 && timeNew[timeNewI] < 0) {
        timeNew[timeNewI] = 12;
      } else if (timeNewI == 2 && timeNew[timeNewI] < 0) {
        timeNew[timeNewI] = 31;
      } else if (timeNewI == 3 && timeNew[timeNewI] < 0) {
        timeNew[timeNewI] = 23;
      } else if ((timeNewI == 4 || timeNewI == 5) && timeNew[timeNewI] < 0) {
        timeNew[timeNewI] = 59;
      }
    }
  }
  if (bitRead(jButtons, 13)) {
    if (timeNewI <= 0) {
      timeNewI = 6;
    } else {
      timeNewI--;
    }
  }
  if (bitRead(jButtons, 14)) {
    if (timeNewI == 7) {
      rtc.adjust(DateTime(timeNew[0], timeNew[1], timeNew[2], timeNew[3], timeNew[4], timeNew[5]));
    } else {
      menuOpen(255);
    }
  }
}

String timeStr() {
  char r[19];
  DateTime now = rtc.now();
  sprintf(r, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  r[19] = '\0';
  return r;
}

