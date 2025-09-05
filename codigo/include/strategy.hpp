#pragma once

class Strategy {
public:
  virtual void poll();
  virtual void button_a_tap();
  virtual void button_b_tap();
};
