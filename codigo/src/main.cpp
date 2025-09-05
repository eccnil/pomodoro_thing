#include "button.hpp"
#include <Arduino.h>
#include <avr/io.h>
#include <button.hpp>
#include <segments.hpp>
#include <shift_register.hpp>
#include <strategies/conter_strategy.hpp>
#include <strategies/pomodoro_strategy.hpp>
#include <strategy.hpp>
#include <util/delay.h>

// gpio PINs (using alternative order)
#define SER 8   // PA2 physical 11
#define RCLK 9  // PA1 physical 12
#define MR 10   // PA0 physical 13
#define LATCH 0 // PB0 physical 2

#define BUTTON_A 7 // PA3 physical 10
#define BUTTON_B 3 // PA7 Physical 6

// global variables
Strategy *strategy;

// shiftRegiseter that drives the display inputs
shiftRegisterPins srPins = {
    .latch = LATCH, .data = SER, .clock = RCLK, .clear = MR};
ShiftRegister shift_register = ShiftRegister(srPins, BITS_TO_WRITE);

// display (4digits x 7 sgements + dot)
Segments display = Segments(shift_register);

// press button_a (right one)
Button button_a = Button(BUTTON_A);
void b_a() { strategy->button_a_tap(); }

// press button_a (left one)
Button button_b = Button(BUTTON_B);
void b_b() { strategy->button_b_tap(); }

#define DEVICE_QUANTITY 3
Device *devices[DEVICE_QUANTITY] = {&display, &button_a, &button_b};

// strategies
Strategy counter = CounterStrategy(display, button_a, button_b);
Strategy pomodoro = PomodoroStrategy(display);

void setup() {
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->init();
  }

  strategy = button_b.is_pressed() ? &counter : &pomodoro;

  button_a.set_on_tap(b_a);
  button_b.set_on_tap(b_b);
}

void loop() {
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->poll();
  };

  strategy->poll();
}
