// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "enum_error.h"
#include "goldbach_calculator.h"
#include "goldbach_thread.h"
#include "number_container.h"
#include "numbers_array.h"
#include "prime_container.h"

// procedure main (thread_count)
int main(int argc, char * argv[]) {
  enum error_t error = ERROR_SUCCESS;

  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2 && sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
  } else {
    if (argc > 2) {
      error = INPUT_ERROR;
    }
  }

  if (error == ERROR_SUCCESS) {
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
        // setup_threads(thread_count, numbers_array, primes_array)
        error = goldbach_thread_setup_threads(thread_count, &array, &primes);

        if (error == ERROR_SUCCESS) {
          view_numbers(&array);
        }
      }
      prime_container_destroy(&primes);
    }
    numbers_array_destroy(&array);
  }

  if (error != EXIT_SUCCESS) {
    show_error(error);
  }

  return error;
}
// end procedure
