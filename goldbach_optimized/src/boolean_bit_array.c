
// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "boolean_bit_array.h"

#include <assert.h>

#include <stdio.h>

enum error_t bit_array_init(struct bit_array * array, uint64_t size) {
  enum error_t error = ERROR_SUCCESS;

  array->sieve = (uint8_t*) calloc(size, sizeof(uint8_t));
  if (array->sieve == NULL) {
    array->sieve = 0;
    error = MEMORY_ERROR;
  }

  return error;
}

enum error_t bit_array_destroy(struct bit_array * array) {
  assert(array);
  free(array->sieve);

  return ERROR_SUCCESS;
}

enum error_t bit_array_set_true(struct bit_array * array, uint64_t number) {
  assert(array);
  // Contains mask positions from 0-7
  // 128 = 1000 0000
  uint8_t masks[]= {128, 64, 32, 16, 8, 4, 2, 1};

  // Replace with an or with mask to insert in position
  // Divides number by 2^3 or 8
  // This specifies which position in array wants to be accessed
  uint64_t position = number >> 3;

  // Residual division to determine which bit position is desired
  // number % 8 can be done by doing a bitwise AND with 7
  // number & 8 == number & 7
  array->sieve[position] = array->sieve[position] | masks[number & 7];

  return ERROR_SUCCESS;
}

enum error_t bit_array_set_false(struct bit_array * array, uint64_t number) {
  assert(array);
  uint8_t masks[]= {128, 64, 32, 16, 8, 4, 2, 1};
  // Residual division to determine which bit position is desired
  // number & 8 can be done by doing a bitwise AND with 7
  uint8_t residue = number & 7;
  // Divides number by 2^3 or 8
  // This specifies which position in array wants to be accessed
  uint64_t position = number >> 3;

  if ((masks[residue] & array->sieve[position]) > 0) {
    // Replace with an xor with mask to insert in position
    array->sieve[position] = array->sieve[position] - masks[residue];
  }

  return ERROR_SUCCESS;
}

bool bit_array_get_value(struct bit_array * array, uint64_t number) {
  assert(array);
  bool answer = false;
  uint8_t masks[]= {128, 64, 32, 16, 8, 4, 2, 1};
  // Residual division to determine which bit position is desired
  // number & 8 can be done by doing a bitwise AND with 7
  uint8_t residue = number & 7;
  // Divides number by 2^3 or 8
  // This specifies which position in array wants to be accessed
  uint64_t position = number >> 3;

  if ((masks[residue] & array->sieve[position]) > 0) {
    answer = true;
  }

  return answer;
}

bool bit_array_xor(struct bit_array * array, uint64_t number) {
  // Simulates sieve[n] ^= true in original version;
  bool answer = bit_array_get_value(array, number);
  answer = answer ^ true;
  if (answer) {
    bit_array_set_true(array, number);
  } else {
    bit_array_set_false(array, number);
  }
  return answer;
}
