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

Segments display = Segments(shift_register);

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
  display.init();

  pinMode(pinButton, INPUT_PULLUP); // Button pin

  delay(200);
  num = 1;
}

void loop() {

  display.set_number(num, buttonPressed ? 4 : 0);

  checkForButtonPress();
  display.poll();
}
