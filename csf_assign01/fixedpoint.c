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
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return 0;
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
