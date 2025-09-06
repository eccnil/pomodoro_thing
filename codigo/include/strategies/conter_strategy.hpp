
#pragma once
#include <devices/button.hpp>
#include <devices/segments.hpp>
#include <strategy.hpp>

class CounterStrategy : public Strategy {
  int num = 0;
  Button &button_a, &button_b;
  void increment_num() { num++; }
  void decrement_num() { num--; }
  int dot_counter_display() {
    int dot = 0;
    if (button_a.is_pressed())
      dot = 4;
    if (button_b.is_pressed())
      dot = 1;
    if (button_a.is_pressed() && button_b.is_pressed())
      dot = 2;
    return dot;
  }

public:
  CounterStrategy(Button &a, Button &b) : button_a(a), button_b(b) {}
  void button_b_tap() override { decrement_num(); }
  void button_a_tap() override { increment_num(); }
  void display(Segments &display) override {
    display.set_number(num, dot_counter_display());
  };
};
