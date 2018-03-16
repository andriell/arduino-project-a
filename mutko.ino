// Ш
byte mutko152[8] = {
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b11111,
  0b00000,
};
// Щ
byte mutko153[8] = {
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b11111,
  0b00001,
  0b00000,
};
// Ъ
byte mutko154[8] = {
  0b11000,
  0b01000,
  0b01000,
  0b01110,
  0b01001,
  0b01001,
  0b01110,
  0b000000,
};
// Ы
byte mutko155[8] = {
  0b10001,
  0b10001,
  0b10001,
  0b11101,
  0b10101,
  0b10101,
  0b11101,
  0b00000,
};
// Ь
byte mutko156[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b11100,
  0b10010,
  0b10010,
  0b11100,
  0b00000,
};
// Э
byte mutko157[8] = {
  0b11100,
  0b00010,
  0b00001,
  0b01111,
  0b00001,
  0b00010,
  0b11100,
  0b00000,
};
// Ю
byte mutko158[8] = {
  0b10010,
  0b10101,
  0b10101,
  0b10101,
  0b11101,
  0b10101,
  0b10010,
  0b00000,
};
// Я
byte mutko159[8] = {
  0b01111,
  0b10001,
  0b10001,
  0b01111,
  0b01001,
  0b10001,
  0b10001,
  0b00000,
};

String mutko(String str) {
  char r[str.length() + 1];
  str.getBytes(r, str.length() + 1);
  for (int i = 0; i <= str.length(); i++) {
    r[i] = mutkoReplace(str[i]);
  }
  r[str.length()] = '\0';
  return r;
}

void mytkoLcdCreateChar(LiquidCrystal_I2C lcd) {
  lcd.createChar(0, mutko152);
  lcd.createChar(1, mutko153);
  lcd.createChar(2, mutko154);
  lcd.createChar(3, mutko155);
  lcd.createChar(4, mutko156);
  lcd.createChar(5, mutko157);
  lcd.createChar(6, mutko158);
  lcd.createChar(7, mutko159);
}

byte mutkoReplace(byte b) {
  // a-z => A-Z
  if (b >= 97 && b <= 122) {
    return b - 32;
  }
  // а-п => А-П
  if (b >= 160 && b <= 175) {
    b -= 32;
  }
  // р-я => Р-Я
  if (b >= 224 && b <= 239) {
    b -= 80;
  }
  // А-Я,Ё,ё
  if (!((b >= 128 && b <= 159) || b == 240 || b == 241)) {
    return b;
  }
  // К-П => K-P
  if (b >= 138 && b <= 143) {
    return b - 63;
  }
  // Ш-Я
  if (b >= 152 && b <= 159) {
    return b - 152;
  }

  switch (b) {
    case 128:
      return 65;
    case 129:
      return 66;
    case 130:
      return 86;
    case 131:
      return 71;
    case 132:
      return 68;
    case 133:
      return 69;
    case 134:
      return 74;
    case 135:
      return 90;
    case 136:
      return 73;
    case 137:
      return 73;
    case 144:
      return 82;
    case 145:
      return 83;
    case 146:
      return 84;
    case 147:
      return 89;
    case 148:
      return 70;
    case 149:
      return 88;
    case 150:
      return 67;
    case 151:
      return 72;
    case 240:
      return 69;
    case 241:
      return 69;
  }
}
