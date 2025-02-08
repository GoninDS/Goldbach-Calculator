// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "enum_error.h"
#include "goldbach_calculator.h"
#include "number_container.h"
#include "numbers_array.h"
#include "prime_container.h"

// procedure main
int main(void) {
  enum error_t error;
  // numbers_array as a dynamic array of number_container
  struct numbers_array array;
  numbers_array_init(&array);
  // obtain_numbers(numbers_array)
  error = goldbach_calculator_obtain_numbers(&array);

  if (error == ERROR_SUCCESS) {
    // primes_array as a dynamic array of prime_container
    struct prime_container primes;
    prime_container_init(&primes);
    // setup_primes(numbers_array, primes_array)
    error = goldbach_calculator_setup_primes(&array, &primes);

    if (error == ERROR_SUCCESS) {
      // calculate_sums(numbers_array, primes_array)
      error = goldbach_calculator_calculate_sums(&array, &primes);
      if (error == ERROR_SUCCESS) {
        view_numbers(&array);
      }
    }
    prime_container_destroy(&primes);
  }

  if (error != EXIT_SUCCESS) {
    show_error(error);
  }

  numbers_array_destroy(&array);

  return 0;
}
// end procedure
