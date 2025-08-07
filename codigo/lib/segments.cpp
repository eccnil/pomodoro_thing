#include <segments.hpp>

int compose_digit(int num, bool dot, int pos) {
  if (dot) {
    return num | S_DOT | pos;
  } else {
    return num | pos;
  }
}
