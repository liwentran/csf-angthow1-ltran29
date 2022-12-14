/*
 * Functions defining the operations of fixed point values.
 * CSF Assignment 1
 * Liwen Tran, Anthony Sky Ng-Thow-Hing
 * ltran29@jhu.edu, angthow1@jhu.edu
 */

#ifndef FIXEDPREC_H
#define FIXEDPREC_H

#include <stdint.h>

typedef struct {
  uint64_t whole;
  uint64_t frac;
  uint8_t tag;  // view big flag reference below
} Fixedpoint;

// Tag Bit Flag Reference
// Most significant bit
//   0b100000 | is_negative (0 if its non-negative, 1 if negative)
//   0b010000 | is_error (0 if valid, 1 if error)
//   0b001000 | is_positive_overflow (1 if positive overflow else 0)
//   0b000100 | is_negative_overflow (1 if negative overflow else 0)
//   0b000010 | is_positive_undeflow (1 if positive underflow else 0)
//   0b000001 | is_negative_undeflow (1 if negative underflow else 0)
// Least significant bit



// Create a Fixedpoint value representing an integer.
//
// Parameters:
//   whole - the value of the whole part of the representation
//
// Returns:
//   the Fixedpoint value
Fixedpoint fixedpoint_create(uint64_t whole); // both

// Create a Fixedpoint value from specified whole and fractional values.
//
// Parametrs:
//   whole - the value of the whole part of the representation
//   frac - the value of the fractional part of the representation, where
//          the highest bit is the halves (2^-1) place, the second highest
//          bit is the fourths (2^-2) place, etc.
//
// Returns:
//   the Fixedpoint value
Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac); // both

// Create a Fixedpoint value from a string representation.
// The string will have one of the following forms:
//
//    X
//    -X
//    X.Y
//    -X.Y
//
// In all value strings, X and Y are sequences of 0 to 16 hex digits
// (chosen from 0-9, a-f, A-F).
//
// Returns:
//   if the string is valid, the Fixedpoint value;
//   if the string is invalid, a Fixedpoint value for which
//   fixedpoint_is_err returns true
Fixedpoint fixedpoint_create_from_hex(const char *hex); // liwen

// Get the whole part of the given Fixedpoint value.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   a uint64_t value which is the whole part of the Fixedpoint value
uint64_t fixedpoint_whole_part(Fixedpoint val); // both

// Get the fractional part of the given Fixedpoint value.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   a uint64_t value which is the fractional part of the Fixedpoint value
uint64_t fixedpoint_frac_part(Fixedpoint val); // both

// Compute the sum of two valid Fixedpoint values.
//
// Parameters:
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//   if the sum left + right is in the range of values that can be represented
//   exactly, the sum is returned;
//   if the sum left + right is not in the range of values that can be
//   represented, then a value for which either fixedpoint_is_overflow_pos or
//   fixedpoint_is_overflow_neg returns true is returned (depending on whether
//   the overflow was positive or negative)
Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right); // sky

// Helper function for fixedpoint_add that computes the sum of two valid Fixedpoint values with the same sign.
//
// Parameters:
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//   if the sum left + right is in the range of values that can be represented
//   exactly, the sum is returned;
//   if the sum left + right is not in the range of values that can be
//   represented, then a value for which either fixedpoint_is_overflow_pos or
//   fixedpoint_is_overflow_neg returns true is returned (depending on whether
//   the overflow was positive or negative)
Fixedpoint fixedpoint_add_same_sign(Fixedpoint left, Fixedpoint right); 

// Helper function for fixedpoint_add that computes the sum of two valid Fixedpoint values with different signs.
//
// Parameters:
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//   if the sum left + right is in the range of values that can be represented
//   exactly, the sum is returned;
//   if the sum left + right is not in the range of values that can be
//   represented, then a value for which either fixedpoint_is_overflow_pos or
//   fixedpoint_is_overflow_neg returns true is returned (depending on whether
//   the overflow was positive or negative)
Fixedpoint fixedpoint_add_dif_sign(Fixedpoint left, Fixedpoint right); 



// Compute the difference of two valid Fixedpoint values.
//
// Parameters:
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//   if the difference left - right is in the range of values that can be represented
//   exactly, the difference is returned;
//   if the difference left - right is not in the range of values that can be
//   represented, then a value for which either fixedpoint_is_overflow_pos or
//   fixedpoint_is_overflow_neg returns true is returned (depending on whether
//   the overflow was positive or negative)
Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right); // sky

// Negate a valid Fixedpoint value.  (I.e. a value with the same magnitude but
// the opposite sign is returned.)  As a special case, the zero value is considered
// to be its own negation.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Returns:
//   the negation of val
Fixedpoint fixedpoint_negate(Fixedpoint val); // liwen

// Return a Fixedpoint value that is exactly 1/2 the value of the given one.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Return:
//   a Fixedpoint value exactly 1/2 of the given one, if it can be represented exactly;
//   otherwise, a Fixedpoint value for which either fixedpoint_is_underflow_pos
//   or fixedpoint_is_underflow_neg returns true (depending on whether the
//   computed value would have been positive or negative)
Fixedpoint fixedpoint_halve(Fixedpoint val); // liwen

// Return a Fixedpoint value that is exactly twice the value of the given one.
//
// Parameters:
//   val - a valid Fixedpoint value
//
// Return:
//   a Fixedpoint value exactly twice the given one, if it can be represented exactly;
//   otherwise, a Fixedpoint value for which either fixedpoint_is_overflow_pos
//   or fixedpoint_is_overflow_neg returns true (depending on whether the
//   computed value would have been positive or negative)
Fixedpoint fixedpoint_double(Fixedpoint val); // liwen

// Compare two valid Fixedpoint values.
//
// Parameters:
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//    -1 if left < right;
//     0 if left == right;
//     1 if left > right
int fixedpoint_compare(Fixedpoint left, Fixedpoint right); // liwen

// Determine whether a Fixedpoint value is equal to 0.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is a valid Fixedpoint value equal to 0;
//   0 is val is not a valid Fixedpoint value equal to 0
int fixedpoint_is_zero(Fixedpoint val); // both

// Determine whether a Fixedpoint value is an "error" value resulting
// from a call to fixedpoint_create_from_hex for which the argument
// string was invalid.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of a call to fixedpoint_create_from_hex with
//   an invalid argument string;
//   0 otherwise
int fixedpoint_is_err(Fixedpoint val); // liwen

// Determine whether a Fixedpoint value is negative (less than 0).
//
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is a valid value less than 0;
//   0 otherwise
int fixedpoint_is_neg(Fixedpoint val); // liwen

// Determine whether a Fixedpoint value is the result of negative overflow.
// Negative overflow results when a sum, difference, or product is negative
// and has a magnitude that is too large to represent.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where negative overflow occurred;
//   0 otherwise
int fixedpoint_is_overflow_neg(Fixedpoint val); // liwen

// Determine whether a Fixedpoint value is the result of positive overflow.
// Positive overflow results when a sum, difference, or product is positive
// and has a magnitude that is too large to represent.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where positive overflow occurred;
//   0 otherwise
int fixedpoint_is_overflow_pos(Fixedpoint val); // liwen

// Determine whether a Fixedpoint value is the result of negative underflow.
// Negative underflow occurs when a division (i.e., fixedpoint_halve)
// produces a value that is negative, and can't be exactly represented because
// the fractional part of the representation doesn't have enough bits.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where negative underflow occurred;
//   0 otherwise
int fixedpoint_is_underflow_neg(Fixedpoint val); // liwen

// Determine whether a Fixedpoint value is the result of positive underflow.
// Positive underflow occurs when a division (i.e., fixedpoint_halve)
// produces a value that is positive, and can't be exactly represented because
// the fractional part of the representation doesn't have enough bits.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val is the result of an operation where positive underflow occurred;
//   0 otherwise
int fixedpoint_is_underflow_pos(Fixedpoint val); // liwen

// Determine whether a Fixedpoint value represents a valid negative or non-negative number.
//
// Parameters:
//   val - the Fixedpoint value
//
// Returns:
//   1 if val represents a valid negative or non-negative number;
//   0 otherwise
int fixedpoint_is_valid(Fixedpoint val); //liwen

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
char *fixedpoint_format_as_hex(Fixedpoint val);

/*
* Turns a hex character to its numeric value.
* If the passed in value is not between 0-9 or a-f, 
* returns -1. 
*
* Parameters:
*   c - a character representing a number in hexidecimal
*
* Returns:
*   the integer value of the hexidecimal character 
*/
uint8_t hex_to_int(uint8_t c);

/*
* Appends a given int to an existing string
* in reverse order. 
*
* Parameters:
*   value - the uint64_t value to reformat as hex
*   is_frac - boolean value indicating if this is the fractional part (1 for frac)
*   s - the array of character to add the hex characters to
*   idx - the index that we want to start appending the hex string to
*/
void append_int_to_string(uint64_t value, int is_frac, char *s, int *idx);

/*
* Reverses a string
*
* Parameters:
*   s - the string to reverse
*/
void reverse_string(char *s);
#endif // FIXEDPREC_H


