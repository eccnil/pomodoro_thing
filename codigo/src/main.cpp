#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Hello, World!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Looping...");
  delay(1000); // Delay for a second
}
