// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "number_container.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "enum_error.h"

#define INITIAL_CAPACITY 100
#define INCREASE_FACTOR 10

/**
 @brief En caso de ocupar más memoria para las sumas, la aumenta
 @param num_container Contiene los datos que se quieren manipular
 @see enum_error.h
*/
enum error_t number_container_memory_increase(
  struct number_container * num_container);

/**
 @brief Muestra las sumas de un número par
 @param num_container Contiene el puntero con las sumas
*/
void view_even(struct number_container * num_container);

/**
 @brief Muestra las sumas de un número impar
 @param num_container Contiene el puntero con las sumas
*/
void view_odd(struct number_container * num_container);

// procedure init()
enum error_t number_container_init(struct number_container * num_container) {
  assert(num_container);
  enum error_t error = ERROR_SUCCESS;
  // number := 0 base case
  num_container->number = 0;
  // sum_total := 0 Manages amount of sums
  num_container->sum_amount = 0;

  // is_even := 1  0 is an even number
  num_container->is_even = true;
  // is_valid := 1
  num_container->is_valid = true;
  // see_sums := 0;
  num_container->see_sums = false;

  // size_used := 0
  // memory_size := 0
  num_container->count = 0;
  num_container->capacity = INITIAL_CAPACITY;
  return error;
}
// end procedure

// procedure set_number (inserted_number)
enum error_t number_container_set_number(
  struct number_container * num_container, int64_t number) {
  assert(num_container);
  enum error_t error = ERROR_SUCCESS;
  if (number < 0) {
    num_container->goldbach_sums = (uint64_t*) calloc(
      num_container->capacity, sizeof(uint64_t));
    num_container->see_sums = true;
    // number := inserted_number
    num_container->number = number * -1;
    // Hecho de manera generica ya que es para varios tipos de datos
    // container as array of desired data type
    if (num_container->goldbach_sums == NULL) {
      num_container->capacity = 0;
      error = MEMORY_ERROR;
    }
  } else {
    // number := inserted_number
    num_container->number = number;
  }

  // if (number can not be divided by 2)
  if ((number & 1) != 0) {  // Se inicializa como 0
    // is_even := 0
    num_container->is_even = false;
  }
  return error;
}
// end procedure

// procedure destroy()
enum error_t number_container_destroy(struct number_container * num_container) {
  // free memory in charge of container
  if (num_container->see_sums) {
    free(num_container->goldbach_sums);
  }
  // Release memory consumed by sums
  // clear()
  number_container_clear(num_container);
  return ERROR_SUCCESS;
}
// end procedure

enum error_t number_container_clear(struct number_container * num_container) {
  assert(num_container);

  // number := 0 Base case
  num_container->number = 0;
  // sum_total := 0 Manages amount of sums
  num_container->sum_amount = 0;
  // is_even := 1 0 is an even number
  num_container->is_even = true;
  // is_valid := 1
  num_container->is_valid = true;
  num_container->see_sums = false;

  // size_used := 0
  num_container->count = 0;
  // memory_size := 0
  num_container->capacity = INITIAL_CAPACITY;
  return ERROR_SUCCESS;
}

// procedure insert(data)
enum error_t number_container_insert(struct number_container * num_container,
  uint64_t number) {
  assert(num_container);
  enum error_t error = ERROR_SUCCESS;
  if (num_container->see_sums) {
    if (num_container->count == num_container->capacity) {
    // memory_increase()
    error = number_container_memory_increase(num_container);
  }
  if (error == EXIT_SUCCESS) {
    // sums[size_used-1] = number
    num_container->goldbach_sums[num_container->count++] = number;
    // Increment size_used by 1
  }
  }
  return error;
}
// end procedure

// procedure memory_increase()
enum error_t number_container_memory_increase(
  struct number_container * num_container) {
  assert(num_container);
  enum error_t error = ERROR_SUCCESS;
  int64_t new_capacity =
    INCREASE_FACTOR * (num_container->capacity ? num_container->capacity : 1);
  uint64_t * new_sums = (uint64_t*) realloc(
    num_container->goldbach_sums, new_capacity * sizeof(uint64_t));
  if (new_sums) {
    num_container->capacity = new_capacity;
    num_container->goldbach_sums = new_sums;
  } else {
    error = MEMORY_ERROR;
  }
  return error;
}

// end procedure

void view_number(struct number_container * num_container) {
  assert(num_container);

  int64_t number = num_container->number;
  uint64_t amount = num_container->sum_amount;
  bool not_valid = false;

  if (num_container->see_sums == true && num_container->is_valid == true) {
    printf("-%" PRId64 ": %" PRIu64 " sums: ", number, amount);
  } else {
    if (num_container->is_valid == true) {
      printf("%" PRId64 ": %" PRIu64 " sums", number, amount);
    } else {
      if (num_container->see_sums == true) {
        printf("-%" PRId64 ": NA", number);
      } else {
        printf("%" PRId64 ": NA", number);
      }

      not_valid = true;
    }
  }

  if (!not_valid && num_container->see_sums == true) {
    if (num_container->is_even) {
      view_even(num_container);
    } else {
      view_odd(num_container);
    }
  }
}

void view_even(struct number_container * num_container) {
  size_t i = 0;
  for (i = 0; i < num_container -> count - 2; i += 2) {
    printf("%" PRIu64 " + %" PRIu64 ", ", num_container->goldbach_sums[i],
      num_container->goldbach_sums[i+1]);
  }
  printf("%" PRIu64 " + %" PRIu64 "", num_container->goldbach_sums[i],
    num_container->goldbach_sums[i+1]);
}

void view_odd(struct number_container * num_container) {
  size_t i = 0;
  for (i = 0; i < num_container -> count - 3; i += 3) {
    printf("%" PRIu64 " + %" PRIu64 " + %" PRIu64 ", ",
      num_container->goldbach_sums[i],
      num_container->goldbach_sums[i+1],
      num_container->goldbach_sums[i+2]);
  }
  printf("%" PRIu64 " + %" PRIu64 " + %" PRIu64 "",
    num_container->goldbach_sums[i],
    num_container->goldbach_sums[i+1],
    num_container->goldbach_sums[i+2]);
}

