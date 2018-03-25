#define THERMO_PIN 8
#define THERMO_PRECISION 11
#define THERMO_SENSOR_COUNT 3
const int THERMO_PIN_0 = A3;

OneWire oneWire(THERMO_PIN);
DallasTemperature sensors(&oneWire);

DeviceAddress thermometer[3] =
{
  { 0x28, 0xFF, 0x10, 0xD1, 0xC1, 0x17, 0x4, 0xA1 },
  { 0x28, 0xFF, 0x99, 0xE3, 0xC1, 0x17, 0x5, 0x88 },
  { 0x28, 0xFF, 0xA5, 0xEC, 0xC1, 0x17, 0x5, 0xB7 }
};
float thermo[4] = {0, 0, 0, 0};

int thermoCalibration[103];
void thermoSetupCalibration() {
    thermoCalibration[6] = 1246;
    thermoCalibration[7] = 1458;
    thermoCalibration[8] = 1652;
    thermoCalibration[9] = 1897;
    thermoCalibration[10] = 2102;
    thermoCalibration[11] = 2315;
    thermoCalibration[12] = 2523;
    thermoCalibration[13] = 2751;
    thermoCalibration[14] = 2970;
    thermoCalibration[15] = 3261;
    thermoCalibration[16] = 3466;
    thermoCalibration[17] = 3677;
    thermoCalibration[18] = 3935;
    thermoCalibration[19] = 4133;
    thermoCalibration[20] = 4358;
    thermoCalibration[21] = 4530;
    thermoCalibration[22] = 4739;
    thermoCalibration[23] = 4916;
    thermoCalibration[24] = 5082;
    thermoCalibration[25] = 5251;
    thermoCalibration[26] = 5396;
    thermoCalibration[27] = 5558;
    thermoCalibration[28] = 5709;
    thermoCalibration[29] = 5860;
    thermoCalibration[30] = 5993;
    thermoCalibration[31] = 6134;
    thermoCalibration[32] = 6267;
    thermoCalibration[33] = 6407;
    thermoCalibration[34] = 6530;
    thermoCalibration[35] = 6658;
    thermoCalibration[36] = 6789;
    thermoCalibration[37] = 6926;
    thermoCalibration[38] = 7054;
    thermoCalibration[39] = 7165;
    thermoCalibration[40] = 7312;
    thermoCalibration[41] = 7425;
    thermoCalibration[42] = 7550;
    thermoCalibration[43] = 7680;
    thermoCalibration[44] = 7793;
    thermoCalibration[45] = 7902;
    thermoCalibration[46] = 8032;
    thermoCalibration[47] = 8157;
    thermoCalibration[48] = 8262;
    thermoCalibration[49] = 8388;
    thermoCalibration[50] = 8506;
    thermoCalibration[51] = 8642;
    thermoCalibration[52] = 8757;
    thermoCalibration[53] = 8876;
    thermoCalibration[54] = 9007;
    thermoCalibration[55] = 9139;
    thermoCalibration[56] = 9269;
    thermoCalibration[57] = 9381;
    thermoCalibration[58] = 9530;
    thermoCalibration[59] = 9653;
    thermoCalibration[60] = 9783;
    thermoCalibration[61] = 9891;
}

void thermoSetup() {
  lcdLog("Thermo loading...");
  thermoSetupCalibration();
  pinMode(THERMO_PIN_0, INPUT);

  sensors.begin();

  int deviceCount = sensors.getDeviceCount();
  lcdLogInt("Thermo count ", deviceCount);

  DeviceAddress tempDeviceAddress;
  char tempDeviceAddressStr[20];

  for (int i = 0; i < deviceCount; i++)
  {
    if (sensors.getAddress(tempDeviceAddress, i))
    {
      sprintf(tempDeviceAddressStr, "%02d-%02X%02X%02X%02X%02X%02X%02X%02X", i, tempDeviceAddress[0], tempDeviceAddress[1], tempDeviceAddress[2], tempDeviceAddress[3], tempDeviceAddress[4], tempDeviceAddress[5], tempDeviceAddress[6], tempDeviceAddress[7]);
      tempDeviceAddressStr[19] = '\0';
      lcdLog(tempDeviceAddressStr);
    } else {
      lcdLogInt("Not found thermo  ", i);
    }
  }

  for (int i = 0; i < THERMO_SENSOR_COUNT; i++) {
    sensors.setResolution(thermometer[i], THERMO_PRECISION);
  }
  lcdLog("Thermo loaded");
}

void thermoLoop() {
  int i = thermoT0Int();
  int key = floor(((float) i) / 10);
  thermo[0] = 0;
  if (key > 0 && key < 101) {
    thermo[0] = ((float) map(i, key * 10, (key + 1) * 10, thermoCalibration[key], thermoCalibration[key + 1])) / 100;
  }

  sensors.requestTemperatures();
  for (byte i = 0; i < THERMO_SENSOR_COUNT; i++) {
    thermo[i + 1] = sensors.getTempC(thermometer[i]);
  }
}

int thermoT0Int() {
  return analogRead(THERMO_PIN_0);
}

float thermoT0() {
  return thermo[0];
}

// Cooler
float thermoTC() {
  return thermo[2];
}

// Tank
float thermoTT() {
  return thermo[1];
}

// Coolant 
float thermoTOZ() {
  return thermo[3];
}
