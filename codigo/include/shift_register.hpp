#pragma once
#include <Arduino.h>
#include <device.h>

#define DELAY 1

struct shiftRegisterPins {
  int latch;
  int data;
  int clock;
  int clear;
};

class ShiftRegister : public Device {
  int pin_latch, pin_data, pin_clock, pin_clear, bits_to_write;
  void init_pins();
  void init_levels();
  void write_bit(bool value);
  void write_bits(int bits);
  void show();

public:
  ShiftRegister(shiftRegisterPins pins, int size_to_write);
  void init() override;
  void display_bits(int);
};

inline void ShiftRegister::init_pins() {
  pinMode(pin_data, OUTPUT);
  pinMode(pin_clock, OUTPUT);
  pinMode(pin_clear, OUTPUT);
  pinMode(pin_latch, OUTPUT);
}

inline void ShiftRegister::init_levels() {
  digitalWrite(pin_data, LOW);
  digitalWrite(pin_clock, LOW);
  digitalWrite(pin_clear, HIGH);
  digitalWrite(pin_latch, LOW);
}

inline void ShiftRegister::write_bit(bool value) {
  digitalWrite(pin_clock, LOW);
  digitalWrite(pin_data, value ? HIGH : LOW);
  digitalWrite(pin_clock, HIGH);
}

inline void ShiftRegister::write_bits(int bits) {
  for (int i = bits_to_write - 1; i >= 0; i--) {
    write_bit((bits >> i) & 1);
  }
}

inline ShiftRegister::ShiftRegister(shiftRegisterPins pins, int size_to_write) {
  pin_clear = pins.clear;
  pin_latch = pins.latch;
  pin_clock = pins.clock;
  pin_data = pins.data;
  bits_to_write = size_to_write;
}

inline void ShiftRegister::init() {
  init_pins();
  init_levels();
}

inline void ShiftRegister::show() {
  digitalWrite(pin_latch, HIGH);
  delay(DELAY);
  digitalWrite(pin_latch, LOW);
}

inline void ShiftRegister::display_bits(int bits) {
  write_bits(bits);
  show();
}
