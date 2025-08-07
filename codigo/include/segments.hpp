#pragma once

#define BITS_TO_WRITE 8 + 4

#define SEG_A 0b0000000000000001
#define SEG_B 0b0000000000000010
#define SEG_C 0b0000000000000100
#define SEG_D 0b0000000000001000
#define SEG_E 0b0000000000010000
#define SEG_F 0b0000000000100000
#define SEG_G 0b0000000001000000
#define S_DOT 0b0000000010000000
#define DIG_1 0b0000111000000000
#define DIG_2 0b0000110100000000
#define DIG_3 0b0000101100000000
#define DIG_4 0b0000011100000000

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
}
