#pragma once

#include <device.h>
#include <shift_register.hpp>

// segments definitions
#define BITS_TO_WRITE 8 + 4 + 3
#define NUM_DIGITS 4
// segments   ----afed.--cg---b-
// digitn n   ---1---------423--  <- digits are active-low, so negate unwanted
#define SEG_A 0b0010000000000000
#define SEG_B 0b0000000000000010
#define SEG_C 0b0000000001000000
#define SEG_D 0b0000010000000000
#define SEG_E 0b0000100000000000
#define SEG_F 0b0001000000000000
#define SEG_G 0b0000000000100000
#define S_DOT 0b0000001000000000
#define DIG_1 0b0000000000011100
#define DIG_2 0b0100000000010100
#define DIG_3 0b0100000000011000
#define DIG_4 0b0100000000001100

#define SDIG_0 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define SDIG_1 (SEG_B | SEG_C)
#define SDIG_2 (SEG_A | SEG_B | SEG_G | SEG_E | SEG_D)
#define SDIG_3 (SEG_A | SEG_B | SEG_G | SEG_C | SEG_D)
#define SDIG_4 (SEG_F | SEG_B | SEG_G | SEG_C)
#define SDIG_5 (SEG_A | SEG_F | SEG_G | SEG_C | SEG_D)
#define SDIG_6 (SEG_A | SEG_F | SEG_G | SEG_C | SEG_D | SEG_E)
#define SDIG_7 (SEG_A | SEG_B | SEG_C)
#define SDIG_8 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_G | SEG_F)
#define SDIG_9 (SEG_A | SEG_B | SEG_C | SEG_G | SEG_F)
#define SDIG_BLANK 0

/** crea la serie de bits necesaria para
 * escribir un digito numerico en uno de los
 * 4 numeros
 *
 * requiere
 * el numero
 * si quieres o no punto decimal
 * el digito del display que quieres encender
 */
int compose_digit(int num, bool dot, int pos);

int compose_digit(int num, bool dot, int pos) {
  if (dot) {
    return num | S_DOT | pos;
  } else {
    return num | pos;
  }
};

struct segment_bits {
  int a, b, c, d, e, f, g, dot;
} def_segment_bits = {.a = SEG_A,
                      .b = SEG_B,
                      .c = SEG_C,
                      .d = SEG_D,
                      .e = SEG_E,
                      .f = SEG_F,
                      .g = SEG_G,
                      .dot = S_DOT};
struct digit_bits {
  int d1, d2, d3, d4;
} def_digit_bits = {.d1 = DIG_1, .d2 = DIG_2, .d3 = DIG_3, .d4 = DIG_4};

class Segments : public Device {
  digit_bits d;
  int current_bits[NUM_DIGITS];
  ShiftRegister &sr;
  int numberSegments[10];

  // calculate the segment to lit any number (0-9) based on defined a-g+dt
  // segments
  void init_number_segments();

public:
  segment_bits s;

  /// constructor
  Segments(ShiftRegister &shift_register,
           segment_bits segments = def_segment_bits,
           digit_bits digits = def_digit_bits);

  /// stores the digit bits for a given position
  void set_digit_bits(int bits, int pos);
  void set_digit_number(int number, bool dot, int pos);
  // TODO: setnumber
  // void set_number(int number, int dot_pos = 0);

  /** inits the shift register */
  void init() override;

  /** displays the stored elements via shift register, one afther other */
  void poll() override;
};

inline void Segments::init_number_segments() {
  numberSegments[0] = s.a | s.b | s.c | s.d | s.e | s.f;
  numberSegments[1] = s.b | s.c;
  numberSegments[2] = s.a | s.b | s.g | s.e | s.d;
  numberSegments[3] = s.a | s.b | s.g | s.c | s.d;
  numberSegments[4] = s.f | s.b | s.g | s.c;
  numberSegments[5] = s.a | s.f | s.g | s.c | s.d;
  numberSegments[6] = s.a | s.f | s.g | s.c | s.d | s.e;
  numberSegments[7] = s.a | s.b | s.c;
  numberSegments[8] = s.a | s.b | s.c | s.d | s.e | s.g | s.f;
  numberSegments[9] = s.a | s.b | s.c | s.g | s.f;
}

inline Segments::Segments(ShiftRegister &sreg, segment_bits sb, digit_bits db)
    : d(db), sr(sreg), s(sb) {
  init_number_segments();
}

inline void Segments::set_digit_bits(int bits, int pos) {
  int index = pos - 1;
  switch (pos) {
  case 1:
    current_bits[index] = d.d1 | bits;
    break;
  case 2:
    current_bits[index] = d.d2 | bits;
    break;
  case 3:
    current_bits[index] = d.d3 | bits;
    break;
  case 4:
    current_bits[index] = d.d4 | bits;
    break;
  }
}

inline void Segments::set_digit_number(int number, bool dot, int pos) {
  int numberbits;
  if (number >= 0 && number <= 9)
    numberbits = numberSegments[number];
  else
    numberbits = 0;
  if (dot)
    numberbits = numberbits | s.dot;
  set_digit_bits(numberbits, pos);
}

inline void Segments::init() { sr.init(); }

inline void Segments::poll() {
  for (int i = 0; i < NUM_DIGITS; i++) {
    sr.display_bits(current_bits[i]);
  }
}
