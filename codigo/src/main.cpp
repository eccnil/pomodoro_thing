#include <Arduino.h>
#include <avr/io.h>
#include <devices/button.hpp>
#include <devices/segments.hpp>
#include <devices/shift_register.hpp>
#include <strategies/conter_strategy.hpp>
#include <strategies/metronome.hpp>
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
void b_a() {
  if (strategy != NULL)
    strategy->button_a_tap();
}

// press button_a (left one)
Button button_b = Button(BUTTON_B);
void b_b() {
  if (strategy != NULL)
    strategy->button_b_tap();
}

#define DEVICE_QUANTITY 3
Device *devices[DEVICE_QUANTITY] = {&display, &button_a, &button_b};

void setup() {
  // init devices
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->init();
  }

  // check buttons to decide the functionality to be loaded
  button_a.poll();
  button_b.poll();
  if (button_b.is_pressed()) {
    strategy = (Strategy *)new CounterStrategy(button_a, button_b);
  } else if (button_a.is_pressed()) {
    strategy = (Strategy *)new Metronome();
  } else {
    strategy = (Strategy *)new PomodoroStrategy();
  }

  // set buttons actions to the selected functionality
  button_b.set_on_tap(b_b);
  button_a.set_on_tap(b_a);
}

void loop() {
  // update all devices
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->poll();
  };

  // update strategy
  strategy->display(display);
}
