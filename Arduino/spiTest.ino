/* SPI Test */

// include the SPI library:
#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

void setup() {
  pinMode(slaveSelectPin, OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  SPI.begin();
}

void loop() {
  char buf[32] = {};
  sprintf(buf, "Hello Arduino world please work");
  /*
  delay(1500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1500);
  digitalWrite(LED_BUILTIN, LOW);
  */
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(10, LOW);
  SPI.transfer(buf, sizeof(buf));
  digitalWrite(10, HIGH);
  SPI.endTransaction();
  delay(500);
}
