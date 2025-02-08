// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "goldbach_calculator.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "boolean_bit_array.h"
#include "enum_error.h"
#include "number_container.h"
#include "numbers_array.h"

/**
 @brief Calcula las sumas de un número par dado
 @param number Struct que contiene información del número
 @param sieve Struct para consultar los números primos
 @see enum_error.h
*/
enum error_t goldbach_calculator_odd_case(struct number_container * number,
  struct bit_array * sieve);

/**
 @brief Calcula las sumas de un número impar dado
 @see goldbach_calculator_odd_case
 @see enum_error.h
*/
enum error_t goldbach_calculator_even_case(struct number_container * number,
  struct bit_array * sieve);

// procedure obtain_numbers(number_array of number_container)
enum error_t goldbach_calculator_obtain_numbers(struct numbers_array * array) {
  assert(array);
  enum error_t error = ERROR_SUCCESS;
  int correctly_read = 0;
  int64_t number_obtained = 0;
  bool continue_read = true;
  errno = 0;

  // while continue_read do
  while (error == EXIT_SUCCESS && continue_read) {
    // read number
    // read continue_read
    correctly_read = scanf("%" SCNd64, &number_obtained);
    if (correctly_read == 1 && errno == 0) {
      // insert(number)  Inserte en number_array
      numbers_array_insert(array, number_obtained, -1);
    } else {
      if (correctly_read == EOF && errno == 0) {
        continue_read = false;
      } else {
        fprintf(stderr, "invalid input\n");
        error = ERROR_READ_VALUES;
        continue_read = false;
      }
    }
  }
  return error;
}
// end procedure

uint64_t goldbach_calculator_highest_number(struct numbers_array * array) {
  assert(array);
  uint64_t highest_number = 0;
  if (array->count > 0) {
    // highest_number := number_array[0]
    highest_number = array->contained_numbers[0].number;
    // for index := 0 to elements do
    for (size_t i = 0; i < array->count; ++i) {
      // if (number_array[index] > highest_number)
      if (array->contained_numbers[i].number > (int64_t)highest_number) {
        // highest_number = number_array[index]
        highest_number = array->contained_numbers[i].number;
      }
    }  // Se sale con el highest number, se utiliza para conseguir los primos
  }
  return highest_number;
}

// procedure calculate_number(current_goldbach, primes)
enum error_t goldbach_calculator_calculate_number(struct number_container
  * number_container, struct bit_array * sieve) {
  assert(number_container);
  assert(sieve);
  enum error_t error = ERROR_SUCCESS;
  // if current_number > 5
  if (number_container->number > 5) {
    // if current_number is even
    if (number_container->is_even) {
      // even_case(current_goldbach, primes)
      error = goldbach_calculator_even_case(number_container, sieve);
    } else {
      // odd_case(current_goldbach, primes)
      error = goldbach_calculator_odd_case(number_container, sieve);
    }
  } else {
    // Se toma el 4 como caso especial, debido a los casos de prueba nuevos
    // if current_number != 4
    if (number_container->number != 4) {
      // current_goldbach is not valid
      number_container->is_valid = false;
    } else {
      if (number_container->see_sums) {
        number_container_insert(number_container, 2);
        number_container_insert(number_container, 2);
      }
      number_container->sum_amount = 1;
    }
  }
  return error;
}
// end procedure

// procedure even_case(current_number, primes)
enum error_t goldbach_calculator_even_case(
  struct number_container * current_number,
  struct bit_array * sieve) {
  enum error_t error = ERROR_SUCCESS;
  int64_t number = current_number->number;

  for (int64_t prime = 3; prime <= current_number->number/2; ++prime) {
    if (bit_array_get_value(sieve, prime)) {
      number = current_number->number - prime;
      if (bit_array_get_value(sieve, number)) {
        ++current_number->sum_amount;
        error = number_container_insert(current_number, prime);
        error = number_container_insert(current_number, number);
      }
    }
  }

  return error;
}
// end procedure

// procedure odd_case(current_number, primes)
enum error_t goldbach_calculator_odd_case(
  struct number_container * current_number,
  struct bit_array * sieve) {
  assert(sieve);
  assert(current_number);

  enum error_t error = ERROR_SUCCESS;
  int64_t number = current_number->number;
  int64_t aux = 0;

  for (int64_t first_prime = 2;
    first_prime <= current_number->number/2; ++first_prime) {
    number = current_number->number;
    if (bit_array_get_value(sieve, first_prime)) {
      number -= first_prime;
      for (int64_t second_prime = first_prime;
        second_prime <= number/2; ++second_prime) {
        aux = number;
        if (bit_array_get_value(sieve, second_prime)) {
          aux -= second_prime;
          if (aux < 0) {
            break;
          }
          if (bit_array_get_value(sieve, aux)) {
            ++current_number->sum_amount;
            error = number_container_insert(current_number, first_prime);
            error = number_container_insert(current_number, second_prime);
            error = number_container_insert(current_number, aux);
          }
        }
      }
    }
  }

  return error;
}

enum error_t calculate_sieve(struct bit_array * sieve,
  uint64_t limit) {
  if (sieve) {
    if (limit > 2) {
      bit_array_set_true(sieve, 2);
    }
    if (limit > 3) {
      bit_array_set_true(sieve, 3);
    }

    for (uint64_t x = 1; x * x <= limit; ++x) {
      for (uint64_t y = 1; y * y <= limit; ++y) {
        uint64_t n = (4 * x * x) + (y * y);
        if (n <= limit && (n % 12 == 1 || n % 12 == 5)) {
          bit_array_xor(sieve, n);
        }

        n = (3 * x * x) + (y * y);
        if (n <= limit && n % 12 == 7) {
          bit_array_xor(sieve, n);
        }

        n = (3 * x * x) - (y * y);
        if (x > y && n <= limit && n % 12 == 11) {
          bit_array_xor(sieve, n);
        }
      }
    }

    for (uint64_t r = 5; r * r <= limit; r++) {
      if (bit_array_get_value(sieve, r)) {
        for (uint64_t i = r * r; i <= limit; i += r * r) {
          bit_array_set_false(sieve, i);
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
