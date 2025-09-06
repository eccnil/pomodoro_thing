#pragma once
#include <devices/segments.hpp>

class Strategy {
public:
  virtual void display(Segments &s);
  virtual void button_a_tap();
  virtual void button_b_tap();
};
