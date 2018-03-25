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
    thermoCalibration[3] = -70;
    thermoCalibration[4] = 67;
    thermoCalibration[5] = 204;
    thermoCalibration[6] = 341;
    thermoCalibration[7] = 478;
    thermoCalibration[8] = 615;
    thermoCalibration[9] = 752;
    thermoCalibration[10] = 889;
    thermoCalibration[11] = 1026;
    thermoCalibration[12] = 1163;
    thermoCalibration[13] = 1300;
    thermoCalibration[14] = 1437;
    thermoCalibration[15] = 1574;
    thermoCalibration[16] = 1711;
    thermoCalibration[17] = 1848;
    thermoCalibration[18] = 1985;
    thermoCalibration[19] = 2122;
    thermoCalibration[20] = 2259;
    thermoCalibration[21] = 2396;
    thermoCalibration[22] = 2533;
    thermoCalibration[23] = 2670;
    thermoCalibration[24] = 2807;
    thermoCalibration[25] = 2944;
    thermoCalibration[26] = 3081;
    thermoCalibration[27] = 3218;
    thermoCalibration[28] = 3355;
    thermoCalibration[29] = 3492;
    thermoCalibration[30] = 3516;
    thermoCalibration[31] = 3686;
    thermoCalibration[32] = 3830;
    thermoCalibration[33] = 3975;
    thermoCalibration[34] = 4164;
    thermoCalibration[35] = 4378;
    thermoCalibration[36] = 4605;
    thermoCalibration[37] = 4811;
    thermoCalibration[38] = 4995;
    thermoCalibration[39] = 5214;
    thermoCalibration[40] = 5464;
    thermoCalibration[41] = 5778;
    thermoCalibration[42] = 6116;
    thermoCalibration[43] = 6501;
    thermoCalibration[44] = 6900;
    thermoCalibration[45] = 7240;
    thermoCalibration[46] = 7548;
    thermoCalibration[47] = 7772;
    thermoCalibration[48] = 7976;
    thermoCalibration[49] = 8170;
    thermoCalibration[50] = 8346;
    thermoCalibration[51] = 8497;
    thermoCalibration[52] = 8656;
    thermoCalibration[53] = 8796;
    thermoCalibration[54] = 8942;
    thermoCalibration[55] = 9086;
    thermoCalibration[56] = 9219;
    thermoCalibration[57] = 9373;
    thermoCalibration[58] = 9508;
    thermoCalibration[59] = 9620;
    thermoCalibration[60] = 9774;
    thermoCalibration[61] = 9854;
    thermoCalibration[62] = 10126;
    thermoCalibration[63] = 10268;
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
  return thermo[1];
}

// Tank
float thermoTT() {
  return thermo[2];
}

// Coolant 
float thermoTOZ() {
  return thermo[3];
}
