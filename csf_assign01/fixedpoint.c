#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

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
  // tracks if hex string contains minus sign
  int has_minus_sign = 0;
  int is_error = 0;
  if (*hex == '-') {
    has_minus_sign = 1;
    hex++;
  }

  uint64_t whole = 0;
  uint64_t frac = 0;
  int count = 0;

  uint64_t *val = &whole;

  while (*hex) {
    uint8_t c = *hex++;
    count += 1;

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
      count = 0;
      continue;
    } else {
      // invalid character, so tag as invalid
      is_error = 1;
      break;
    }

    // too many characters
    if (count > 16) {
      is_error = 1;
      break;
    }
    // shift the existing bits to the left to make space
    *val = (*val << 4) | (c);
  }

  // if we hex string has frac part, make sure that we fill the rest with zero
  if (val == &frac && count < 16) {
    *val = *val << (16 - count)*4;
  }
  
  // create the fixedpoint and edit tag
  Fixedpoint fp = fixedpoint_create2(whole,frac);

  // set is_negative if hex string had negative sign and value is not zero. 
  if (has_minus_sign && (whole != 0 || frac != 0)) {
    fp.tag = 1 << 5;
  }

  // set tag as error
  if (is_error) {
    fp.tag |= 1 << 4;
  }
  
  return fp;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  Fixedpoint sum = fixedpoint_create(0);
  if (fixedpoint_is_neg(left) ^ fixedpoint_is_neg(right)) {   //positive + negative (subtract)
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
  }
  else{   //if left and right are both positive or both negative
    sum.frac = left.frac + right.frac;
    if(sum.frac < left.frac || sum.frac < right.frac){     //check for carry
      sum.whole = 1;
    }
    sum.tag = left.tag;
    sum.whole += left.whole + right.whole;
    if(sum.whole < left.whole || sum.whole < right.whole){     //check overflow
      if(fixedpoint_is_neg(sum)){
        sum.tag |= 1 << 2; //overflow negative
      }
      else{
        sum.tag |= 1 << 3; //overflow positive
      }
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
  int result = 0;

  if (left.whole == right.whole) {
    if (left.frac == right.frac) return 0;
    else if (left.frac > right.frac) result = 1;
    else result = -1;
  } else {
    if (left.whole > right.whole) result = 1;
    else result = -1;
  }

  // If both are positive return 1 when left > right
  if (!fixedpoint_is_neg(left)  && !fixedpoint_is_neg(right)) {
    return result;
  
  // If both are negative return 1 when abs(left) < abs(right)
  } else if (fixedpoint_is_neg(left) && fixedpoint_is_neg(right)) {
    return result * -1;
    
  } else {
    // if one value is negative, the positive value is greater (return 1 if right is neg)
    return (fixedpoint_is_neg(right)) ? 1 : -1;
  }

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

// Return a dynamically allocated C character string with the representation of
// the given valid Fixedpoint value.  The string should start with "-" if the
// value is negative, and should use the characters 0-9 and a-f to represent
// each hex digit of the whole and fractional parts. As a special case, if the
// Fixedpoint value represents an integer (i.e., the fractional part is 0),
// then no "decimal point" ('.') should be included.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   dynamically allocated character string containing the representation
//   of the Fixedpoint value
char *fixedpoint_format_as_hex(Fixedpoint val) {
  char *s = malloc(34); // 16 whole, 16 frac values, 1 negative sign, 1 decimal point 
  int idx = 0; // index of array
  uint64_t hex;

  // if string is invalid, return invalid. 
  if (!fixedpoint_is_valid(val)) {
    strcpy(s, "<invalid>");
    return s;
  }

  

  //convert fractional part
  int count = 0;
  int start = 0;
  if(val.frac != 0){
    while(val.frac != 0){
      hex = val.frac & 15; // 0b1111
      if (hex != 0) {
        start = 1;
      }
      if (start){
        if(hex < 10){
          s[idx++] = 48 + hex;
        }
        else{
          s[idx++] = 87 + hex;
        }
      }
      
      count += 1;
      val.frac = val.frac >> 4;
    }
    while (count++ < 16) {
        s[idx++] = '0';
    }
    s[idx++] = '.';
  }

  //convert whole part
  if(val.whole == 0){
    s[idx++] = '0';
  }
  while(val.whole != 0){
    hex = val.whole & 15; // 0b1111
    if(hex < 10){
      s[idx++] = 48 + hex;
    }
    else{
      s[idx++] = 87 + hex;
    }
    val.whole = val.whole >> 4;
  }


  // add - if neg
  if(fixedpoint_is_neg(val)){
    s[idx++] = '-';
  }

  // end string
  s[idx] = '\0';

  // reverse the string
  for(size_t i = 0; i < strlen(s) / 2; i++){
    char temp = s[i];
    s[i] = s[strlen(s) - 1 - i];
    s[strlen(s) - 1 - i] = temp;
  }  
  
  return s;
}
