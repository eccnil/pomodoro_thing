#include <Arduino.h>
#include <avr/io.h>
#include <segments.hpp>
#include <shift_register.hpp>
#include <util/delay.h>

// gpio PINs (using alternative order)
#define SER 8   // PA2 physical 11
#define RCLK 9  // PA1 physical 12
#define MR 10   // PA0 physical 13
#define LATCH 0 // PB0 physical 2
int num = 0;
bool buttonPressed = false;
int pinButton = 7; // pa3 physical 10

shiftRegisterPins srPins = {
    .latch = LATCH, .data = SER, .clock = RCLK, .clear = MR};
ShiftRegister shift_register = ShiftRegister(srPins, BITS_TO_WRITE);

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
  shift_register.init();

  pinMode(pinButton, INPUT_PULLUP); // Button pin

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
  shift_register.display_bits(compose_digit(SDIG_8, false, DIG_1));

  // escribir digito
  shift_register.display_bits(compose_digit(SDIG_BLANK, true, DIG_2));

  // escribir digito
  shift_register.display_bits(compose_digit(SDIG_0, false, DIG_3));

  // escribir digito
  int bits = num2bits4display(num % 10);
  shift_register.display_bits(compose_digit(bits, buttonPressed, DIG_4));

  // delay(1000); // Wait for 1000 millisecond(s)
  checkForButtonPress();
}
