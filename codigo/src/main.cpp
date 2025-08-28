#include "button.hpp"
#include "pomodoro.cpp"
#include <Arduino.h>
#include <avr/io.h>
#include <button.hpp>
#include <segments.hpp>
#include <shift_register.hpp>
#include <util/delay.h>

// gpio PINs (using alternative order)
#define SER 8   // PA2 physical 11
#define RCLK 9  // PA1 physical 12
#define MR 10   // PA0 physical 13
#define LATCH 0 // PB0 physical 2

#define BUTTON_A 7 // PA3 physical 10
#define BUTTON_B 3 // PA7 Physical 6

// global variables
int num = 0; // number to be displayed
void increment_num() { num++; }
void decrement_num() { num--; }

// shiftRegiseter that drives the display inputs
shiftRegisterPins srPins = {
    .latch = LATCH, .data = SER, .clock = RCLK, .clear = MR};
ShiftRegister shift_register = ShiftRegister(srPins, BITS_TO_WRITE);

// display (4digits x 7 sgements + dot)
Segments display = Segments(shift_register);

// press button_a (right one)
Button button_a = Button(BUTTON_A);

// press button_a (left one)
Button button_b = Button(BUTTON_B);

#define DEVICE_QUANTITY 3
Device *devices[DEVICE_QUANTITY] = {&display, &button_a, &button_b};

void setup() {
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->init();
  }

  button_a.set_on_tap(increment_num);
  button_b.set_on_tap(decrement_num);
}

void loop() {
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->poll();
  }

  int dot = 0;
  if (button_a.is_pressed())
    dot = 4;
  if (button_b.is_pressed())
    dot = 1;
  if (button_a.is_pressed() && button_b.is_pressed())
    dot = 2;

  display.set_number(num, dot);
}
