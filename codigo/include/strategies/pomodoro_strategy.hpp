
#pragma once
#include <devices/segments.hpp>
#include <pomodoro.hpp>
#include <strategy.hpp>

class PomodoroStrategy : public Strategy {
  Pomodoro pomodoro;
  void display_pomodoro(Segments &display) {
    Pomodoro::PomodoroDisplay pd = pomodoro.display();

    if (pd.num > 9999 || pd.num < 0) {
      display.show_err();
    } else {
      display.set_digit_number(pd.num % 10, pd.dots & 0b0001, 4);
      display.set_digit_number((pd.num / 10) % 10, pd.dots & 0b0010, 3);
      display.set_digit_number((pd.num / 100) % 10, pd.dots & 0b0100, 2);
      display.set_digit_number((pd.num / 1000) % 10, pd.dots & 0b1000, 1);
    }
  }

public:
  PomodoroStrategy() : pomodoro() {}
  void button_a_tap() override { pomodoro.buttonA(); }
  void button_b_tap() override { pomodoro.buttonB(); }
  void display(Segments &display) override {
    pomodoro.poll(millis());
    display_pomodoro(display);
  }
};
