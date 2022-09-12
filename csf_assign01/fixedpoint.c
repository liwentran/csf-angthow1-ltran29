/*
 * Functions defining the operations of fixed point values.
 * CSF Assignment 1
 * Liwen Tran, Anthony Sky Ng-Thow-Hing
 * ltran29@jhu.edu, angthow1@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"


Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = 0; // fraction defaults to 0
  fp.tag = 0; // tag defaults to 0
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;
  fp.tag = 0; // tag defaults to 0
  return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  // tracks if hex string contains minus sign
  int has_minus_sign = 0, is_error = 0, count = 0;

  // if the string has a negative sign, flag has_minus_sign, and move the pointer
  if (*hex == '-') {
    has_minus_sign = 1;
    hex++;
  }
  
  uint64_t whole = 0, frac = 0;
  uint64_t *val = &whole;
  
  while (*hex) {
    uint8_t c = *hex++;
    count++;

    // find the decimal representation from the character
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
      c = hex_to_int(c);
    } else if (c == '.' && val == &whole) {
      // once we hit the decimal point, find value of frac part, and reset the count
      val = &frac;
      count = 0;
      continue;
    } else {
      // invalid character, so tag as invalid
      is_error = 1;
      break;
    }

    // tag error if too many characters
    if (count > 16) {
      is_error = 1;
      break;
    }
    // shift the existing bits to the left to make space for the next char
    *val = (*val << 4) | (c);
  }
  if (val == &frac && count < 16) {   // if we hex string has frac part, make sure that we fill the rest with zero
    *val = *val << (16 - count)*4;
  }
  Fixedpoint fp = fixedpoint_create2(whole,frac);   // create the fixedpoint and edit tag
  if (has_minus_sign && (whole != 0 || frac != 0)) {   // set is_negative if hex string had negative sign and value is not zero. 
    fp.tag = 1 << 5;
  }

  // set tag as error
  if (is_error) fp.tag |= 1 << 4;
  
  return fp;
}

// Helper function to convert hex char to int
uint8_t hex_to_int(uint8_t c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'a' && c <= 'f') {
    return (c - 'a') + 10;
  } else if (c >= 'A' && c <= 'F') {
    return (c - 'A') + 10;
  } else {
    // unrecognized value
    return -1;
  }
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  if (fixedpoint_is_neg(left) ^ fixedpoint_is_neg(right)) { //positive + negative (subtract)
    return fixedpoint_add_dif_sign(left, right);
  }
  else{ //if left and right are both positive or both negative
    return fixedpoint_add_same_sign(left, right);
  }
}

Fixedpoint fixedpoint_add_dif_sign(Fixedpoint left, Fixedpoint right){
  Fixedpoint sum = fixedpoint_create(0);
  if(left.whole > right.whole){     //Subtract right from left if left is bigger
    sum.whole = left.whole - right.whole;
    sum.frac = left.frac - right.frac;
    if(right.frac > left.frac){ //carry
      sum.whole -= 1;
    }
    sum.tag = left.tag;
  }
  else if(right.whole > left.whole){ //Subtract left from right if right is bigger
    sum.whole = right.whole - left.whole;
    sum.frac = right.frac - left.frac;
    if(left.frac > right.frac){ //carry
      sum.whole -= 1;
    }
    sum.tag = right.tag;
  }
  else{ //left and right have same whole
    sum.whole = 0;
    if(left.frac > right.frac){
      sum.frac = left.frac - right.frac;
      sum.tag = left.tag;
    } 
    else if(left.frac > right.frac){
      sum.frac = right.frac - left.frac;
      sum.tag = right.tag;
    } 
    else{
      sum.tag = 0; //valid
      sum.frac = 0; 
    }
  }
  return sum;
}


Fixedpoint fixedpoint_add_same_sign(Fixedpoint left, Fixedpoint right){
    Fixedpoint sum = fixedpoint_create(0);
    sum.frac = left.frac + right.frac;
    if(sum.frac < left.frac || sum.frac < right.frac){ //check for carry
      sum.whole = 1;
    }
    sum.tag = left.tag;
    sum.whole += left.whole + right.whole;
    if(sum.whole < left.whole || sum.whole < right.whole){ //check overflow
      if(fixedpoint_is_neg(sum)){
        sum.tag |= 1 << 2; //overflow negative
      }
      else{
        sum.tag |= 1 << 3; //overflow positive
      }
    }
    return sum;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  return fixedpoint_add(left, fixedpoint_negate(right));
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  // only negate if value is non zero
  if (val.whole != 0 || val.frac != 0) {
    val.tag ^= 1 << 5; // tag XOR 100000
  }
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // if the rightmost bit is 1, then that means there's underflow
  if (val.frac & 1UL) {
    // flip the right underflow depending on neg or positive
    if (fixedpoint_is_neg(val)) {
      val.tag |= 1;
    } else {
      val.tag |= 1 << 1;
    }
  }

  // shift the fractional part
  uint64_t result_frac = val.frac >> 1;

  // move whole's rightmost bit to frac's leftmost bit
  result_frac = (result_frac & ~(1UL << 63)) | ((val.whole & 1) << 63);

  //shift the whole part
  uint64_t result_whole = val.whole >> 1;

  // create the Fixedpoint object
  Fixedpoint fp = fixedpoint_create2(result_whole, result_frac);
  fp.tag = val.tag;

  return fp;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // if the leftmost bit is 1, then that means there's going to be an overflow
  if (val.whole & (1UL<<63)) {
    // flip the right overflow depending on neg or positive
    if (fixedpoint_is_neg(val)) {
      val.tag |= 1 << 2;
    } else {
      val.tag |= 1 << 3;
    }
  }

  // shift the whole part
  uint64_t result_whole = val.whole << 1;

  // move frac's leftmost bit to whole's rightmost bit
  result_whole = (result_whole & ~(1)) | ((val.frac >> 63) & 1);

  //shift the frac part
  uint64_t result_frac = val.frac << 1;

  // create the Fixedpoint object
  Fixedpoint fp = fixedpoint_create2(result_whole, result_frac);
  fp.tag = val.tag;

  return fp;
}


int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // if left is positive and right is negative, return 1
  if (fixedpoint_is_neg(right) && !fixedpoint_is_neg(left)) {
    return 1;
  } else if (!fixedpoint_is_neg(right) && fixedpoint_is_neg(left)) {
    return -1;
  }

  int result = 0;

  // compare the values
  if (left.whole == right.whole) {
    if (left.frac > right.frac) result = 1;
    else if (left.frac < right.frac) result = -1;
    else return 0;
  } else if (left.whole > right.whole) {
    result = 1;
  } else result = -1;

  // If both are negative return 1 when abs(left) < abs(right)
  if (fixedpoint_is_neg(left)) return result * -1;

  return result;
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
  return !(val.tag & 31); // tag & 11111
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  char *s = malloc(34); // 16 whole, 16 frac values, 1 negative sign, 1 decimal point 
  int idx = 0; // index of array

  // if string is invalid, return invalid. 
  if (!fixedpoint_is_valid(val)) {   
    strcpy(s, "<invalid>");
    return s;
  } 

  //convert fractional part of val
  if(val.frac != 0){
    // add the fractional part to the string
    append_int_to_string(val.frac, 1, s, &idx);
    s[idx++] = '.';
  }

  // convert the whole part of val
  if(val.whole == 0){ 
    s[idx++] = '0';
  } else {
    append_int_to_string(val.whole, 0, s, &idx);
  }

  if(fixedpoint_is_neg(val)){   // add '-' if neg
    s[idx++] = '-';
  }

  s[idx] = '\0';   // end string
  reverse_string(s);
  return s;
}

// Helper function to append a provided int value to a string in hex format
void append_int_to_string(uint64_t value, int is_frac, char *s, int *idx) {
  uint64_t hex; 
  // if the value is a frac, don't append extra zeros to the end
  int start = is_frac ? 0 : 1;
  int count = 0;

  // convert the value into hex
  while(value != 0){
      hex = value & 15; // 0b1111 
      // no longer trailing zeros, start adding to the string
      if (hex != 0) start = 1; 
      
      if (start){
        if (hex < 10) s[*idx] = 48 + hex; // 0 - 9
        else s[*idx] = 87 + hex; // a - f
        (*idx)++;
      }
      count++;
      // shift bits in order to add the next char
      value = value >> 4;
  }

  // add zeros
  if (is_frac) {
    while (count++ < 16) {
      s[*idx] = '0';
      (*idx)++;
    }
  }  
}

// helper function to reversed a string
void reverse_string(char *s) {
  for(size_t i = 0; i < strlen(s) / 2; i++){ 
    char temp = s[i];
    s[i] = s[strlen(s) - 1 - i];
    s[strlen(s) - 1 - i] = temp;
  }
}