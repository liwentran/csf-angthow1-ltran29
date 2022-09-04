#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;
//struct Fixedpoint fp;

Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = 0;
  fp.tag = 0;
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;
  fp.tag = 0;
  return fp;
}

// assuming X and Y are 0-16 digits, we will not check for overflow
// TODO: test if this works
Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  // create negative or positive tag based on first character
  uint8_t hex_tag;
  if (*hex == '-') {
    hex_tag = 1 << 5;
    hex++;
  } else {
    hex_tag = 0;
  }

  uint64_t whole = 0;
  uint64_t frac = 0;

  uint64_t *val = &whole;

  while (*hex) {
    uint8_t c = *hex++;

    // find the decimal representation from the character
    if (c >= '0' && c <= '9') {
      c = c - '0';
    } else if (c >= 'a' && c <= 'f') {
      c = (c - 'a') + 10;
    } else if (c >= 'A' && c <= 'F') {
      c = (c - 'A') + 10;
    } else if (c == '.' && val == &whole) {
      // once we hit the decimal point, find value of frac part
      val = &frac;
      continue;
    } else {
      // invalid character, so tag as invalid
      hex_tag |= 1 << 4;
      break;
    }

    // shift the existing bits to the left to make space
    *val = (*val << 4) | (c);
  }

  // create the fixedpoint and edit tag
  Fixedpoint fp = fixedpoint_create2(whole,frac);
  fp.tag = hex_tag;
  return fp;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  val.tag ^= 1 << 5; // tag XOR 100000
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // represent the whole and half together in one integer
  __uint128_t value = (val.whole << 64) | val.frac;

  // if the rightmost bit is 1, then that means there's underflow
  if (value & 1) {
    // flip the right underflow depending on neg or positive
    if (fixedpoint_is_neg(val)) {
      val.tag |= 1;
    } else {
      val.tag |= 1 << 1;
    }
  }

  // half the value
  value = value >> 1;

  // split value to whole and frac part
  uint64_t result_frac = value & 0xffffffffffffffff;
  uint64_t result_whole = value >> 64;
  Fixedpoint fp = fixedpoint_create2(result_whole, result_frac);
  fp.tag = val.tag;

  return fp;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
    // represent the whole and half together in one integer
  __uint128_t value = (val.whole << 64) | val.frac;

  // if the leftmost bit is 1, then that means there's going to be an overflow
  if (val.whole & (1<<63)) {
    // flip the right underflow depending on neg or positive
    if (fixedpoint_is_neg(val)) {
      val.tag |= 1 << 2;
    } else {
      val.tag |= 1 << 3;
    }
  }

  // half the value
  value = value << 1;

  // split value to whole and frac part
  uint64_t result_frac = value & 0xffffffffffffffff;
  uint64_t result_whole = value >> 64;
  Fixedpoint fp = fixedpoint_create2(result_whole, result_frac);
  fp.tag = val.tag;

  return fp;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // represent the whole and half together in one integer
  __uint128_t left_val = (left.whole << 64) | left.frac;
  __uint128_t right_val = (right.whole << 64) | right.frac;

  // If both are positive return 1 when left > right
  if (!fixedpoint_is_neg(left)  && !fixedpoint_is_neg(right)) {
    if (left_val > right_val) return 1;
    else if (left_val < right_val) return -1;
    else return 0;
  } else if (fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
    // if both are negative, then opposite result
    if (left_val > right_val) return -1;
    else if (left_val < right_val) return 1;
    else return 0;
  }

  // if one value is negative, the positive value is greater (return 1 if right is neg)
  return (fixedpoint_is_neg(right)) ? 1 : -1;
}

int fixedpoint_is_zero(Fixedpoint val) {
  return !val.frac && !val.whole;
}

int fixedpoint_is_err(Fixedpoint val) {
  return (val.tag & 1 << 4) > 0; // tag & 010000
}

int fixedpoint_is_neg(Fixedpoint val) {
  return (val.tag & 1 << 5) > 0; // tag & 100000
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  return (val.tag & 1 << 2) > 0; // tag & 000100
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  return (val.tag & 1 << 3) > 0; // tag & 001000
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  return (val.tag & 1) > 0; // tag & 000001
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  return (val.tag & 1 << 1) > 0; // tag & 000010
}

int fixedpoint_is_valid(Fixedpoint val) {
  return !(val.tag & 0b11111); // tag & 11111
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
