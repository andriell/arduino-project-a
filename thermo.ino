#define THERMO_PIN 8
#define THERMO_PRECISION 11
const int THERMO_PIN_0 = A3;

OneWire oneWire(THERMO_PIN);
DallasTemperature sensors(&oneWire);

DeviceAddress thermometer[4] =
{
  { 0x28, 0xFF, 0x10, 0xD1, 0xC1, 0x17, 0x4, 0xA1 },
  { 0x28, 0xFF, 0xAA, 0xB5, 0xC1, 0x17, 0x4, 0xCD },
  { 0x28, 0xFF, 0x99, 0xE3, 0xC1, 0x17, 0x5, 0x88 },
  { 0x28, 0xFF, 0xA5, 0xEC, 0xC1, 0x17, 0x5, 0xB7 }
};

float* thermo() {
  sensors.requestTemperatures();
  float t[THERMO_SENSOR_COUNT];
  for (byte i = 0; i < THERMO_SENSOR_COUNT; i++) {
    t[i] = sensors.getTempC(thermometer[i]);
  }
  return t;
}

void thermoSetup() {
  lcdLog("Thermo loading...");
  pinMode(THERMO_PIN_0, INPUT);
  
  sensors.begin();
  for (int i = 0; i < THERMO_SENSOR_COUNT; i++) {
    sensors.setResolution(thermometer[i], THERMO_PRECISION);
  }
  lcdLog("Thermo loaded");
}

int thermo0Int() {
  return analogRead(THERMO_PIN_0);
}

