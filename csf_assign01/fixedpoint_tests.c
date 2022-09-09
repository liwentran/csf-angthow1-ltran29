#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint max;

  // TODO: add more objects to the test fixture
} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
// TODO: add more test functions
void my_test_create_from_hex(TestObjs *objs);
void my_test_is_err(TestObjs *objs);
void my_test_halve(TestObjs *objs);
void my_test_double(TestObjs *objs);
void my_test_compare(TestObjs *objs);
void my_test_is_zero(TestObjs *objs);
void my_test_is_overflow_neg(TestObjs *objs);
void my_test_is_underflow_pos(TestObjs *objs);
void my_test_is_underflow_neg(TestObjs *objs);

int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  //TEST(test_negative_sub_negative);
  //TEST(test_add_two_positives);
  TEST(test_sub);
  // TEST(test_is_overflow_pos);
  TEST(test_is_err);

  TEST(my_test_create_from_hex);
  TEST(my_test_is_err);
  TEST(my_test_halve);
  TEST(my_test_double);
  TEST(my_test_compare);
  TEST(my_test_is_zero);
  TEST(my_test_is_overflow_neg);
  TEST(my_test_is_underflow_pos);
  TEST(my_test_is_underflow_neg);
  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");

  ASSERT(fixedpoint_is_valid(val1));

  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));

  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
}


void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  Fixedpoint fp = fixedpoint_create(400);
   s = fixedpoint_format_as_hex(fp);
  ASSERT(0 == strcmp(s, "190"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);


  s = fixedpoint_format_as_hex(fixedpoint_negate(objs->one_half));
  ASSERT(0 == strcmp(s, "-0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  // printf("string: %s\n", s);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  // printf("string: %s\n", s);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  // printf("string: %s\n", s);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
}

void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

/**
void test_add_two_positives(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;
  lhs = fixedpoint_create_from_hex("1AE.16"); //430.22
  rhs = fixedpoint_create_from_hex("1F5.7"); //501.7
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(lhs.whole = 430);
  ASSERT(rhs.whole = 501);
  ASSERT(!fixedpoint_is_neg(sum));
  //printf("frac: %d", fixedpoint_frac_part(sum));
  ASSERT(931UL == fixedpoint_whole_part(sum));
  ASSERT(92UL == fixedpoint_frac_part(sum));
}*/

/**
void test_add_positives(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}
*/

/**
void test_add_negatives(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}
*/

void test_negative_sub_negative(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("-d09079.1e6d601");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(diff));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(diff));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

// TODO: implement more test functions

// My own test to test that create from hex works properly
void my_test_create_from_hex(TestObjs *objs) {
  (void) objs;

  // make sure that uppercase letters work
  Fixedpoint val1 = fixedpoint_create_from_hex("934.ADE8d38A");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(0x934UL == fixedpoint_whole_part(val1));
  ASSERT(0xade8d38a00000000UL == fixedpoint_frac_part(val1));

  // Test to make sure that 0 in frac works
  Fixedpoint val2 = fixedpoint_create_from_hex("934.0");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(0x934UL == fixedpoint_whole_part(val2));
  ASSERT(0 == fixedpoint_frac_part(val2));

  // Test to make sure that 0 in whole works 
  Fixedpoint val3 = fixedpoint_create_from_hex("0.a");
  ASSERT(fixedpoint_is_valid(val3));
  ASSERT(0 == fixedpoint_whole_part(val3));
  ASSERT(0xa000000000000000UL == fixedpoint_frac_part(val3));

  // Test to make sure that 0 in both whole and frac works 
  Fixedpoint val4 = fixedpoint_create_from_hex("0.0");
  ASSERT(fixedpoint_is_valid(val4));
  ASSERT(0 == fixedpoint_whole_part(val4));
  ASSERT(0 == fixedpoint_frac_part(val4));

  // Test to make sure that ending in a decimal still produces valid fp
  Fixedpoint val5 = fixedpoint_create_from_hex("ab2.");
  ASSERT(fixedpoint_is_valid(val5));

  // Test to make sure that a single decimal point produces valid fp 0.0
  Fixedpoint val6 = fixedpoint_create_from_hex(".");
  ASSERT(fixedpoint_is_valid(val6));
  ASSERT(0 == fixedpoint_whole_part(val6));
  ASSERT(0 == fixedpoint_frac_part(val6));
}


void my_test_is_err(TestObjs *objs) {
  (void) objs;

  // two decimal points
  Fixedpoint err1 = fixedpoint_create_from_hex("7..4");
  ASSERT(fixedpoint_is_err(err1));
}

// My own test to test halving
void my_test_halve(TestObjs *objs) {
  //// simple case
  Fixedpoint fp1 = fixedpoint_create_from_hex("-a.3200000000000000");
  Fixedpoint halved = fixedpoint_halve(fp1);

  // Still is negative and valid
  ASSERT(fixedpoint_is_neg(halved));
  ASSERT(fixedpoint_is_valid(halved));

  // value is halved
  ASSERT(0x5UL == fixedpoint_whole_part(halved));
  ASSERT(0x1900000000000000UL == fixedpoint_frac_part(halved));

  //// half of one_half should be one_fourth
  halved = fixedpoint_halve(objs->one_half);

  // Still is positive and valid
  ASSERT(!fixedpoint_is_neg(halved));
  ASSERT(fixedpoint_is_valid(halved));

  // value is halved
  ASSERT(fixedpoint_whole_part(objs->one_fourth) == fixedpoint_whole_part(halved));
  ASSERT(fixedpoint_frac_part(objs->one_fourth) == fixedpoint_frac_part(halved));
}

// My own test to test double
void my_test_double(TestObjs *objs) {
  Fixedpoint doubled, fp;
  
  fp = fixedpoint_create_from_hex("-3c.19");
  doubled = fixedpoint_double(fp);

  // Still is negative and valid
  ASSERT(fixedpoint_is_neg(doubled));
  ASSERT(fixedpoint_is_valid(doubled));

  // value is doubled
  ASSERT(0x78UL == fixedpoint_whole_part(doubled));
  ASSERT(0x3200000000000000UL == fixedpoint_frac_part(doubled));

  // doubling 1/2 should get 1
  doubled = fixedpoint_double(objs->one_half);


  // Still is positive and valid
  ASSERT(!fixedpoint_is_neg(doubled));
  ASSERT(fixedpoint_is_valid(doubled));

  // value is doubled
  ASSERT(1 == fixedpoint_whole_part(doubled));
  ASSERT(0 == fixedpoint_frac_part(doubled));
}

// My own test to test compare
void my_test_compare(TestObjs *objs) {
  // comparing differing whole parts
  ASSERT(fixedpoint_compare(objs->one_half, objs->one) == -1);
  ASSERT(fixedpoint_compare(objs->one, objs->one_half) == 1);

  // comparing same whole parts
  ASSERT(fixedpoint_compare(objs->one_fourth, objs->one_half) == -1);
  ASSERT(fixedpoint_compare(objs->one_half, objs->one_fourth) == 1);

  // comparing same
  ASSERT(fixedpoint_compare(objs->one_half, objs->one_half) == 0);
}

// My own test to test zero
void my_test_is_zero(TestObjs *objs) {
  ASSERT(fixedpoint_is_zero(objs->zero));


  Fixedpoint sum, doubled, halved;

  // adding zero to zero
  sum = fixedpoint_add(objs->zero, objs->zero);
  ASSERT(fixedpoint_is_zero(sum));

  // doubling zero
  doubled = fixedpoint_double(objs->zero);
  ASSERT(fixedpoint_is_zero(doubled));

  // having zero
  halved = fixedpoint_halve(objs->zero);
  ASSERT(fixedpoint_is_zero(halved));
}


// My own test to test overflow neg
void my_test_is_overflow_neg(TestObjs *objs) {

  Fixedpoint sum, doubled;
  Fixedpoint negative_one = fixedpoint_negate(objs->one);
  Fixedpoint min = fixedpoint_negate(objs->max);

  // doubling max
  doubled = fixedpoint_double(min);
  ASSERT(fixedpoint_is_overflow_neg(doubled));

  // adding -1 to min
  sum = fixedpoint_add(min, negative_one);
  ASSERT(fixedpoint_is_overflow_neg(sum));

  // adding min to -1
  sum = fixedpoint_add(negative_one, min);
  ASSERT(fixedpoint_is_overflow_neg(sum));

  // subtracting 1 from min
  sum = fixedpoint_sub(min, objs->one);
  ASSERT(fixedpoint_is_overflow_neg(sum));
}

// My own test to test underflow pos
void my_test_is_underflow_pos(TestObjs *objs) {
  (void) objs;

  Fixedpoint fp1 = fixedpoint_create_from_hex("c7252a193ae07.7a51de9ea0538c51");
  Fixedpoint halved = fixedpoint_halve(fp1);

  // should be positive and underflow neg
  ASSERT(!fixedpoint_is_neg(halved));
  ASSERT(fixedpoint_is_underflow_pos(halved));

}

// My own test to test underflow neg
void my_test_is_underflow_neg(TestObjs *objs) {
  (void) objs;

  Fixedpoint fp1 = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c51");
  Fixedpoint halved = fixedpoint_halve(fp1);

  // should be negative and underflow neg
  ASSERT(fixedpoint_is_neg(halved));
  ASSERT(fixedpoint_is_underflow_neg(halved));
}
