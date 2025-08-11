#pragma once
#include <Arduino.h>
#include <device.h>

class Button : public Device {
  int pin;
  bool pressed;
  void (*on_tap)() = nullptr;

public:
  Button(int gpio_pin);
  void init() override;
  void poll() override;
  void set_on_tap(void (*f)()) { on_tap = f; };
  bool is_pressed() { return pressed; };
};

inline Button::Button(int gpio_pin) {
  pin = gpio_pin;
  pressed = false;
};

inline void Button::init() { pinMode(pin, INPUT_PULLUP); };

inline void Button::poll() {
  auto read = digitalRead(pin);
  if (read == LOW && !pressed) {
    pressed = true; // Button is pressed
    if (on_tap)
      on_tap();
    //  delay(200); // Debounce dela
  } else if (read == HIGH) {
    pressed = false; // Button is released
  }
}
