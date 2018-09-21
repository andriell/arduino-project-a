#include <Wire.h>

void setup() {
  Wire.begin(8);                /* задаем на шине i2c 8 адрес */
  Wire.onReceive(receiveEvent); /* регистрируем полученное событие */
  Serial.begin(9600);           /* открываем серийный порт для дебаггинга */
}
int x = 0;
void loop() {
  delay(1000);
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  x++;
  Serial.println(x);
  delay(1000);
}

// Функция для извлечения любых принимаемых данных от мастера на шину
void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    char c = Wire.read();      /* получаем байт как символ*/
    Serial.print(c);           /* выводим символ в серийный порт */
  }
  Serial.println();             /* переходим на новую строку */
}

