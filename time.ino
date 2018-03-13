int timeNew[6] = {2018, 6, 15, 12, 30, 0};
byte timeNewI = 0;

void timeMenu() {
  oled.print(menuList[1], 5, 10);
  oled.print(timeStr(), 5, 24);

  char timeNewStr[19];
  sprintf(timeNewStr, "%04d-%02d-%02d %02d:%02d:%02d", timeNew[0], timeNew[1], timeNew[2], timeNew[3], timeNew[4], timeNew[5]);
  timeNewStr[19] = '\0';

  oled.print(timeNewStr, 5, 34);

  char* timeNewName[] = {
    "Год",
    "Месяц",
    "День",
    "Час",
    "Минута",
    "Секунда",
    "Сохранить",
    "Выход",
  };

  oled.print(timeNewName[timeNewI], 5, 44);

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
      menuActive = 255;
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

