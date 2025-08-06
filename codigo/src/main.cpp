#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#define SER 1
#define RCLK 2
#define MR 3
#define LATCH 7
#define DELAY 1

void setbit(bool value) {
  digitalWrite(RCLK, LOW);               // clock
  digitalWrite(SER, value ? HIGH : LOW); // data
  digitalWrite(RCLK, HIGH);              // clock
}

void selectDigit(byte digit) {
  for (byte i = 4; i; i--) {
    setbit(!(i == digit));
  }
}

void setbyte(bool values[8]) {
  for (int i = 8; i; i--) {
    bool value = values[i - 1];
    setbit(value);
  }
}

void show() {
  digitalWrite(LATCH, HIGH);
  delay(DELAY); // not needed but good for managing the button
  digitalWrite(LATCH, LOW);
}

void display_digit(byte digit, bool values[8]) {
  selectDigit(digit);
  setbyte(values);
  show();
}

// Function to convert a digit (0-9) into a boolean array (7 segments + DP)
// Returns: segmentArray[8] = {a, b, c, d, e, f, g, dp}
void digitTo7Segment(uint8_t digit, bool includeDot, bool segmentArray[8]) {
  // Default: all segments OFF
  for (int i = 0; i < 8; i++) {
    segmentArray[i] = false;
  }

  // Define segment patterns for digits 0-9 (a, b, c, d, e, f, g)
  switch (digit) {
  case 0:
    segmentArray[0] = true; // a
    segmentArray[1] = true; // b
    segmentArray[2] = true; // c
    segmentArray[3] = true; // d
    segmentArray[4] = true; // e
    segmentArray[5] = true; // f
    break;
  case 1:
    segmentArray[1] = true; // b
    segmentArray[2] = true; // c
    break;
  case 2:
    segmentArray[0] = true; // a
    segmentArray[1] = true; // b
    segmentArray[3] = true; // d
    segmentArray[4] = true; // e
    segmentArray[6] = true; // g
    break;
  case 3:
    segmentArray[0] = true; // a
    segmentArray[1] = true; // b
    segmentArray[2] = true; // c
    segmentArray[3] = true; // d
    segmentArray[6] = true; // g
    break;
  case 4:
    segmentArray[1] = true; // b
    segmentArray[2] = true; // c
    segmentArray[5] = true; // f
    segmentArray[6] = true; // g
    break;
  case 5:
    segmentArray[0] = true; // a
    segmentArray[2] = true; // c
    segmentArray[3] = true; // d
    segmentArray[5] = true; // f
    segmentArray[6] = true; // g
    break;
  case 6:
    segmentArray[0] = true; // a
    segmentArray[2] = true; // c
    segmentArray[3] = true; // d
    segmentArray[4] = true; // e
    segmentArray[5] = true; // f
    segmentArray[6] = true; // g
    break;
  case 7:
    segmentArray[0] = true; // a
    segmentArray[1] = true; // b
    segmentArray[2] = true; // c
    break;
  case 8:
    for (int i = 0; i < 7; i++)
      segmentArray[i] = true; // All segments ON
    break;
  case 9:
    segmentArray[0] = true; // a
    segmentArray[1] = true; // b
    segmentArray[2] = true; // c
    segmentArray[3] = true; // d
    segmentArray[5] = true; // f
    segmentArray[6] = true; // g
    break;
  default: // Invalid digit → turn OFF all segments (except DP if enabled)
    break;
  }

  // Set the decimal point (DP)
  segmentArray[7] = includeDot;
}

int num = 0;
bool buttonPressed = false;
int pinButton = 0;

void checkForButtonPress() {
  auto read = digitalRead(pinButton);
  if (read == LOW && !buttonPressed) {
    buttonPressed = true; // Button is pressed
    num++;
    // delay(200); // Debounce delay
  } else if (read == HIGH) {
    buttonPressed = false; // Button is released
  }
}

void setup() {
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(MR, OUTPUT);
  pinMode(LATCH, OUTPUT);

  pinMode(pinButton, INPUT_PULLUP); // Button pin

  // inicio
  digitalWrite(SER, LOW);
  digitalWrite(RCLK, LOW);
  digitalWrite(MR, HIGH);
  digitalWrite(LATCH, LOW);
  delay(200);
}

void loop() {
  bool dato[8];

  // escribir digito
  digitTo7Segment(8, false, dato);
  display_digit(1, dato);

  // delay(1000); // Wait for 1000 millisecond(s)

  // escribir digito
  digitTo7Segment(10, true, dato);
  display_digit(2, dato);

  // delay(1000); // Wait for 1000 millisecond(s)

  // escribir digito
  digitTo7Segment(7, false, dato);
  display_digit(3, dato);

  // delay(1000); // Wait for 1000 millisecond(s)
  // escribir digito
  digitTo7Segment(num, false, dato);
  display_digit(4, dato);

  // delay(1000); // Wait for 1000 millisecond(s)
  checkForButtonPress();
}
