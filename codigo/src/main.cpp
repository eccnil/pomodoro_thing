#include "button.hpp"
#include "pomodoro.hpp"
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
enum Strategy { counter_strategy, pomodoro_strategy } strategy;

int num = 0; // number to be displayed
void increment_num() { num++; }
void decrement_num() { num--; }

Pomodoro pomodoro = Pomodoro();
void pomodoro_a() { pomodoro.buttonA(); }
void pomodoro_b() { pomodoro.buttonB(); }

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

int dot_counter_display(Button &a, Button &b) {
  int dot = 0;
  if (a.is_pressed())
    dot = 4;
  if (b.is_pressed())
    dot = 1;
  if (a.is_pressed() && b.is_pressed())
    dot = 2;
  return dot;
}

void display_pomodoro(Pomodoro &p, Segments &d) {
  Pomodoro::PomodoroDisplay pd = p.display();

  if (pd.num > 9999 || pd.num < 0) {
    d.show_err();
  } else {
    d.set_digit_number(pd.num % 10, pd.dots & 0b0001, 4);
    d.set_digit_number((pd.num / 10) % 10, pd.dots & 0b0010, 3);
    d.set_digit_number((pd.num / 100) % 10, pd.dots & 0b0100, 2);
    d.set_digit_number((pd.num / 1000) % 10, pd.dots & 0b1000, 1);
  }
}

void setup() {
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->init();
  }

  strategy = button_b.is_pressed() ? counter_strategy : pomodoro_strategy;

  switch (strategy) {
  case counter_strategy:
    button_a.set_on_tap(increment_num);
    button_b.set_on_tap(decrement_num);
    break;
  case pomodoro_strategy:
    button_a.set_on_tap(pomodoro_a);
    button_b.set_on_tap(pomodoro_b);
    break;
  }
}

void loop() {
  for (int i = 0; i < DEVICE_QUANTITY; i++) {
    devices[i]->poll();
  };

  switch (strategy) {
  case Strategy::counter_strategy:
    display.set_number(num, dot_counter_display(button_a, button_b));
    break;
  case Strategy::pomodoro_strategy:
    pomodoro.poll(millis());
    display_pomodoro(pomodoro, display);
    break;
  }
}
