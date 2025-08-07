#include <Arduino.h>
#include <avr/io.h>
#include <segments.hpp>
#include <util/delay.h>

#define DELAY 1

void setbit(bool value) {
  digitalWrite(RCLK, LOW);               // clock
  digitalWrite(SER, value ? HIGH : LOW); // data
  digitalWrite(RCLK, HIGH);              // clock
}

void setbits(int bits) {
  for (int i = BITS_TO_WRITE - 1; i >= 0; i--) {
    setbit((bits >> i) & 1);
  }
}

void show() {
  digitalWrite(LATCH, HIGH);
  delay(DELAY); // not needed but good for managing the button
  digitalWrite(LATCH, LOW);
}

void display_bits(int bits) {
  setbits(bits);
  show();
}

int num = 0;
bool buttonPressed = false;
int pinButton = 3; // pa3 physical 10

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

int num2bits4display(char num) {
  int bits = SDIG_BLANK;
  switch (num) {
  case 0:
    bits = SDIG_0;
    break;
  case 1:
    bits = SDIG_1;
    break;
  case 2:
    bits = SDIG_2;
    break;
  case 3:
    bits = SDIG_3;
    break;
  case 4:
    bits = SDIG_4;
    break;
  case 5:
    bits = SDIG_5;
    break;
  case 6:
    bits = SDIG_6;
    break;
  case 7:
    bits = SDIG_7;
    break;
  case 8:
    bits = SDIG_8;
    break;
  case 9:
    bits = SDIG_9;
    break;
  }
  return bits;
}

void loop() {

  // escribir digito
  display_bits(compose_digit(SDIG_8, false, DIG_1));

  // escribir digito
  display_bits(compose_digit(SDIG_BLANK, true, DIG_2));

  // escribir digito
  display_bits(compose_digit(SDIG_0, false, DIG_3));

  // escribir digito
  int bits = num2bits4display(num % 10);
  display_bits(compose_digit(bits, buttonPressed, DIG_4));

  // delay(1000); // Wait for 1000 millisecond(s)
  checkForButtonPress();
}
