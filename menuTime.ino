int newTime[6] = {2018, 6, 15, 12, 30, 0};
byte newTimeI = 0;

void menuTime() {
  oled.print(menuList[1], 5, 10);
  oled.print(getTime(), 5, 24);

  char newTimeStr[19];
  sprintf(newTimeStr, "%04d-%02d-%02d %02d:%02d:%02d", newTime[0], newTime[1], newTime[2], newTime[3], newTime[4], newTime[5]);
  newTimeStr[19] = '\0';

  oled.print(newTimeStr, 5, 34);

  char* newTimeName[] = {
    "Год",
    "Месяц",
    "День",
    "Час",
    "Минута",
    "Секунда",
    "Сохранить",
    "Выход",
  };

  oled.print(newTimeName[newTimeI], 5, 44);

  if (bitRead(jButtons, 10)) {
    if (newTimeI < 6) {
      newTime[newTimeI]++;
      if (newTimeI == 1 && newTime[newTimeI] > 12) {
        newTime[newTimeI] = 1;
      } else if (newTimeI == 2 && newTime[newTimeI] > 31) {
        newTime[newTimeI] = 1;
      } else if (newTimeI == 3 && newTime[newTimeI] > 23) {
        newTime[newTimeI] = 0;
      } else if ((newTimeI == 4 || newTimeI == 5) && newTime[newTimeI] > 59) {
        newTime[newTimeI] = 0;
      }
    }
  }
  if (bitRead(jButtons, 11)) {
    newTimeI++;
    if (newTimeI > 7) {
      newTimeI = 0;
    }
  }
  if (bitRead(jButtons, 12)) {
    if (newTimeI < 6) {
      newTime[newTimeI]--;
      if (newTimeI == 0 && newTime[newTimeI] < 2000) {
        newTime[newTimeI] = 2000;
      } else if (newTimeI == 1 && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 12;
      } else if (newTimeI == 2 && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 31;
      } else if (newTimeI == 3 && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 23;
      } else if ((newTimeI == 4 || newTimeI == 5) && newTime[newTimeI] < 0) {
        newTime[newTimeI] = 59;
      }
    }
  }
  if (bitRead(jButtons, 13)) {
    if (newTimeI <= 0) {
      newTimeI = 6;
    } else {
      newTimeI--;
    }
  }
  if (bitRead(jButtons, 14)) {
    if (newTimeI == 7) {
      rtc.adjust(DateTime(newTime[0], newTime[1], newTime[2], newTime[3], newTime[4], newTime[5]));
    } else {
      menuActive = 255;
    }
  }
}

String getTime() {
  char r[19];
  DateTime now = rtc.now();
  sprintf(r, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  r[19] = '\0';
  return r;
}

