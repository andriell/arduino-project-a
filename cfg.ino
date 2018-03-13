// чтение
int cfgRead(byte addr) {
  addr *= 2;
  byte raw[2];
  raw[0] = EEPROM.read(addr);
  raw[1] = EEPROM.read(addr + 1);
  int &num = (int&) raw;
  return num;
}

// запись
void cfgWrite(byte addr, int num) {
  addr *= 2;
  byte raw[2];
  (int&) raw = num;
  EEPROM.write(addr, raw[0]);
  EEPROM.write(addr + 1, raw[1]);
}
