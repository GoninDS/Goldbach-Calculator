// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "boolean_bit_array.h"
#include "enum_error.h"
#include "goldbach_calculator.h"
#include "goldbach_thread.h"
#include "number_container.h"
#include "numbers_array.h"

// procedure main (thread_count)
int main(int argc, char * argv[]) {
  enum error_t error = ERROR_SUCCESS;

  // printf("see me \n");

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
    error = numbers_array_init(&array);
    // obtain_numbers(numbers_array)

    if (error == ERROR_SUCCESS) {
      error = goldbach_calculator_obtain_numbers(&array);

      uint64_t highest_number = goldbach_calculator_highest_number(&array);
      struct bit_array sieve_container;
      uint64_t array_size = highest_number / 8;
      ++array_size;
      error = bit_array_init(&sieve_container, array_size);
      if (error == ERROR_SUCCESS) {
        error = calculate_sieve(&sieve_container, highest_number+1);
        if (error == ERROR_SUCCESS) {
          if (sieve_container.sieve) {
            // setup_threads(thread_count, numbers_array, primes_array)
            error = goldbach_thread_setup_threads(thread_count, &array,
              &sieve_container);
            if (error == ERROR_SUCCESS) {
              view_numbers(&array);
            }
          }
          numbers_array_destroy(&array);
          bit_array_destroy(&sieve_container);
        }
      }
    }

    if (error != EXIT_SUCCESS) {
      show_error(error);
    }
  }
  return error;
}
// end procedure
