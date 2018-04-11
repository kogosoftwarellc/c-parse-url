#ifndef TEST_C_H
#define TEST_C_H
#include <stdio.h>
#include <string.h>
#define TEST_C_PASSED 0
#define TEST_C_FAILED 1

#define TEST_C_START(name) \
  int name() {

#define TEST_C_END \
    return TEST_C_PASSED; \
  }

#define TEST_C_ASSERT_CHAR_EQUAL(expected, actual) \
  if (expected != actual) { \
    printf("TEST_C_ASSERT_CHAR_EQUAL failed!  Expected %c was given %c\n", expected, actual); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_INT_EQUAL(expected, actual) \
  if (expected != actual) { \
    printf("TEST_C_ASSERT_INT_EQUAL failed!  Expected %d was given %d\n", expected, actual); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_INT_GT(min, actual) \
  if (actual <= min) { \
    printf("TEST_C_ASSERT_INT_GT failed!  Expected %d to be greater than %d\n", actual, min); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_INT_GTEQ(min, actual) \
  if (actual < min) { \
    printf("TEST_C_ASSERT_INT_GT failed!  Expected %d to be greater than or equal to %d\n", actual, min); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_INT_LT(min, actual) \
  if (actual >= min) { \
    printf("TEST_C_ASSERT_INT_GT failed!  Expected %d to be less than %d\n", actual, min); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_INT_LTEQ(min, actual) \
  if (actual > min) { \
    printf("TEST_C_ASSERT_INT_GT failed!  Expected %d to be less than or equal to %d\n", actual, min); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_LONG_EQUAL(expected, actual) \
  if (expected != actual) { \
    printf("TEST_C_ASSERT_LONG_EQUAL failed!  Expected %ld was given %ld\n", expected, actual); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_NULL(test, msg) \
  if (test != NULL) { \
    printf("TEST_C_ASSERT_NULL failed!  %s\n", msg); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_NOT_NULL(test, msg) \
  if (test == NULL) { \
    printf("TEST_C_ASSERT_NOT_NULL failed!  %s\n", msg); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_STRING_EQUAL(expected, actual) \
  if (strcmp(expected, actual)) { \
    printf("TEST_C_ASSERT_STRING_EQUAL failed!  Expected %s was given %s\n", expected, actual); \
    return TEST_C_FAILED; \
  }

#define TEST_C_ASSERT_STRING_STARTS_WITH(string, proposal) \
  if (strstr(string, proposal) != string) { \
    printf("TEST_C_ASSERT_STRING_STARTS_WITH failed!  Expected %s to begin with %s\n", string, proposal); \
    return TEST_C_FAILED; \
  }


#endif
