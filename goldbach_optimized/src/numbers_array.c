// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "numbers_array.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "enum_error.h"
#include "number_container.h"

#define INITIAL_CAPACITY 100
#define INCREASE_FACTOR 10

/**
 @brief Aumenta la memoria en caso de necesitar más
 @param numbers Contiene el struct que se quiere aumentar
 @see enum_error.h
*/
enum error_t numbers_array_memory_increase(struct numbers_array * numbers);

// Inicialice la memoria
// procedure init ()
enum error_t numbers_array_init(struct numbers_array * numbers) {
  assert(numbers);
  enum error_t error = ERROR_SUCCESS;
  // size_used := 0
  numbers->count = 0;
  // memory_size := 0
  numbers->capacity = INITIAL_CAPACITY;
  // Hecho de manera generica ya que es para varios tipos de datos
  // container as array of desired data type
  numbers->contained_numbers = (struct number_container*) calloc(
    numbers->capacity, sizeof(struct number_container));
  if (numbers->contained_numbers == NULL) {
    error = MEMORY_ERROR;
    numbers->capacity = 0;
  }
  return error;
}
// end procedure

// procedure destroy
enum error_t numbers_array_destroy(struct numbers_array * numbers) {
  assert(numbers);
  // free memory in charge of container
  // En este caso se debe liberar lo que está dentro del arrays
  for (uint64_t i = 0; i < numbers->capacity; ++i) {
    number_container_destroy(&numbers->contained_numbers[i]);
  }
  // free memory in charge of container
  free(numbers->contained_numbers);
  numbers->count = 0;
  numbers->capacity = 0;

  return ERROR_SUCCESS;
}
// end procedure

// procedure insert()
enum error_t numbers_array_insert(struct numbers_array * numbers,
  int64_t number) {
  assert(numbers);
  enum error_t error = EXIT_SUCCESS;
  if (numbers->count == numbers->capacity) {
    // memory_increase()
    // containter[size_used-1] = data
    error = numbers_array_memory_increase(numbers);
  }
  // if (size_used <= memory_size)
  if (error == EXIT_SUCCESS) {
    error = number_container_init(&numbers->contained_numbers[numbers->count]);
    if (error == EXIT_SUCCESS) {
      // Si hay espacio, inserte datos
      // container[size_used-1] = data
      error = number_container_set_number(
        &numbers->contained_numbers[numbers->count], number);
      // Siempre es EXIT_SUCCESS
      // Increment size_used by 1
      numbers->count++;
    }
  }
  return error;
}
// end procedure

// procedure memory_increase
enum error_t numbers_array_memory_increase(struct numbers_array * numbers) {
  assert(numbers);
  enum error_t error = EXIT_SUCCESS;
  int64_t new_capacity =
    INCREASE_FACTOR * (numbers->capacity ? numbers->capacity : 1);
  struct number_container * new_contained_numbers
    = (struct number_container*) realloc(
    numbers->contained_numbers, new_capacity * sizeof(struct number_container));
  if (new_contained_numbers) {
    numbers->capacity = new_capacity;
    numbers->contained_numbers = new_contained_numbers;
  } else {
    error = MEMORY_ERROR;
  }
  return error;
}
// end procedure

void view_numbers(struct numbers_array * numbers) {
  assert(numbers);
  size_t total_numbers = numbers->count;
  size_t total_sums = 0;

  for (size_t i = 0; i < total_numbers; ++i) {
    total_sums += numbers->contained_numbers[i].sum_amount;
  }

  printf("Total %zu numbers %zu sums\n\n", total_numbers, total_sums);

  for (size_t i = 0; i < total_numbers; ++i) {
    view_number(&numbers->contained_numbers[i]);
    printf("\n");
  }
}
