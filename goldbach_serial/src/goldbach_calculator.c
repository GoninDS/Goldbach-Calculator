// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "goldbach_calculator.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "enum_error.h"
#include "number_container.h"
#include "numbers_array.h"
#include "prime_container.h"

/**
 @brief Calcula las sumas de un número par dado
 @param number Struct que contiene información del número
 @param primes Struct para guardar los números primos
 @see enum_error.h
*/
enum error_t goldbach_calculator_odd_case(struct number_container * number,
  struct prime_container * primes);

/**
 @brief Calcula las sumas de un número impar dado
 @see goldbach_calculator_odd_case
 @see enum_error.h
*/
enum error_t goldbach_calculator_even_case(struct number_container * number,
  struct prime_container * primes);

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
      numbers_array_insert(array, number_obtained);
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

// procedure setup_primes(number_array of number_container,
// prime_container, elements)
enum error_t goldbach_calculator_setup_primes(struct numbers_array * array,
  struct prime_container * primes) {
  assert(array);
  assert(primes);
  enum error_t error = ERROR_SUCCESS;
  if (array->count > 0) {
    // highest_number := number_array[0]
    int64_t highest_number = array->contained_numbers[0].number;
    // for index := 0 to elements do
    for (size_t i = 0; i < array->count; ++i) {
      // if (number_array[index] > highest_number)
      if (array->contained_numbers[i].number > highest_number) {
        // highest_number = number_array[index]
        highest_number = array->contained_numbers[i].number;
      }
    }  // Se sale con el highest number, se utiliza para conseguir los primos
    // calculate_primes(prime_container, highest_number)
    error = calculate_primes(primes, highest_number);
  } else {
    error = ILLEGAL_ACCESS;
  }

  return error;
}
// end procedure

// procedure calculate_sums(numbers_array, primes, amount)
enum error_t goldbach_calculator_calculate_sums(struct numbers_array * array,
  struct prime_container * primes) {
  assert(array);
  assert(primes);
  enum error_t error = ERROR_SUCCESS;
  // for index := 0 to amount do
  for (size_t i = 0; i < array->count && (error == ERROR_SUCCESS); ++i) {
    // if numbers_array[index] > 5
    if (array->contained_numbers[i].number > 5) {
      // if numbers_array[index] is even
      if (array->contained_numbers[i].is_even) {
        // even_case(numbers_array[index], primes)
        error = goldbach_calculator_even_case(
          &array->contained_numbers[i], primes);
      } else {
        // odd_case(numbers_array[index], primes)
        error = goldbach_calculator_odd_case(
          &array->contained_numbers[i], primes);
      }
    } else {
      // Se toma el 4 como caso especial, debido a los casos de prueba nuevos
      if (array->contained_numbers[i].number != 4) {
        // numbers_array[index] is not valid
        array->contained_numbers[i].is_valid = false;
      } else {
        error = goldbach_calculator_even_case(
          &array->contained_numbers[i], primes);
      }
    }
  }
  return error;
}
// end procedure

// procedure even_case(current_number, primes)
enum error_t goldbach_calculator_even_case(
  struct number_container * current_number,
  struct prime_container * primes) {
  assert(current_number);
  assert(primes);
  enum error_t error = ERROR_SUCCESS;
  uint64_t number = current_number->number;
  // sum := 0
  uint64_t sum = 0;
  // for first_index := 0 to amount of primes do number
  for (size_t i = 0; i < primes->count && (error == ERROR_SUCCESS); ++i) {
    // for second_index := first_index to amount of primes do
    for (size_t j = i; j < primes->count && (error == ERROR_SUCCESS); ++j) {
      // sum = primes[first_index] + primes[second_index]
      sum = primes->prime_numbers[i]+primes->prime_numbers[j];
      // if sum <= current_number
      if (sum <= number) {
        // if sum >= current_number
        if (sum < number) {
        } else {
          // current_number.insert(primes[first_index])
          error = number_container_insert(
            current_number, primes->prime_numbers[i]);
          // current_numeber.insert(primes[second_index])
          error = number_container_insert(
            current_number, primes->prime_numbers[j]);
          ++current_number->sum_amount;
        }
      } else {
        // Si el segundo num se pasa, no quiero seguir probando
        // second_index := amount of primes
        break;
      }
    }
  }
  return error;
}
// end procedure

// procedure odd_case(current_number, primes)
enum error_t goldbach_calculator_odd_case(
  struct number_container * current_number,
  struct prime_container * primes) {
  assert(current_number);
  assert(primes);

  uint64_t number = current_number->number;
  // sum := 0
  uint64_t sum = 0;
  enum error_t error = ERROR_SUCCESS;

  // for first_index := 0 to amount of primes do
  for (size_t i = 0; i < primes->count && (error == ERROR_SUCCESS); ++i) {
    // for second_index := first_index to amount of primes do
    for (size_t j = i; j < primes->count && (error == ERROR_SUCCESS); ++j) {
      // for third_index := second_index to amount of primes do
      for (size_t k = j; k < primes->count && (error == ERROR_SUCCESS); ++k) {
        // sum := primes[first_index] +
        // primes[second_index] + primes[third_index]
        sum = primes->prime_numbers[i] +
          primes->prime_numbers[j] + primes->prime_numbers[k];
        // if sum <= current_number
        if (sum <= number) {
          // if sum >= current_number
          if (sum < number) {
          } else {
            // current_number.insert(primes[first_index])
            error = number_container_insert(current_number,
              primes->prime_numbers[i]);
            // current_numeber.insert(primes[second_index])
            error = number_container_insert(current_number,
              primes->prime_numbers[j]);
            // current_number.insert(primes[third_index])
            error = number_container_insert(current_number,
              primes->prime_numbers[k]);
            ++current_number->sum_amount;
          }
        } else {
          // Si la suma se pasa, no quiero seguir probando
          // third_index := amount of primes
          break;
        }
      }
    }
  }
  return error;
}
// end procedure

void show_error(enum error_t error) {
  switch (error) {
    case ERROR_SUCCESS:
      // No error to show
    case ERROR_READ_VALUES:
      fprintf(stderr, "Error: could not properly read values\n");
      break;
    case MEMORY_ERROR:
      fprintf(stderr, "Error: could not allocate memory\n");
      break;
    case ILLEGAL_ACCESS:
      fprintf(stderr, "Error: illegal memory acess\n");
      break;
  }
}
