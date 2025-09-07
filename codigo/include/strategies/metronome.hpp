#pragma once
#include "Arduino.h"
#include <devices/segments.hpp>
#include <strategy.hpp>
#include <tempo.hpp>

class Metronome : public Strategy {
  Tempo tempo;
  void display_tempo(int *bits, int tempo, Segments &display);
  void display_phase(int *bits, int phase);

public:
  Metronome() : tempo(){};
  void display(Segments &s) override;
  void button_a_tap() override { tempo.inc_tempo(); };
  void button_b_tap() override { tempo.dec_tempo(); };
};

void Metronome::display(Segments &display) {
  auto info = tempo.show(millis());
  int display_bits[4] = {0, 0, 0, 0};
  display_tempo(&*display_bits, info.bpm, display);
  display_phase(&*display_bits, info.phase);
  for (int i = 0; i < 4; i++) {
    display.set_digit_bits(display_bits[i], i + 1);
  }
}

void Metronome::display_tempo(int *bits, int tempo, Segments &display) {
  int units, decs, cents;
  units = tempo % 10;
  decs = (tempo / 10) % 10;
  cents = (tempo / 100) % 10;

  bits[2] |= display.get_digit_bits(units);
  bits[1] |= display.get_digit_bits(decs);
  bits[0] |= cents == 1 ? display.get_digit_bits(1) : 0;
}

void Metronome::display_phase(int *bits, int phase) {
  switch (phase) {
  case 0:
    bits[0] |= SEG_F;
    bits[0] |= SEG_E;
    break;
  case 4:
    bits[3] |= SEG_B;
    bits[3] |= SEG_C;
    break;
  case 1:
  case 7:
    bits[0] |= S_DOT;
    break;
  case 2:
  case 6:
    bits[1] |= S_DOT;
    break;
  case 3:
  case 5:
    bits[2] |= S_DOT;
    break;
  }
}

// FIXME: el metronomo va el doble de rápido
