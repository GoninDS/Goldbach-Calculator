// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "prime_container.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "enum_error.h"

#define INITIAL_CAPACITY 10
#define INCREASE_FACTOR 10

/**
 @brief En caso de ocupar más memoria para los primos, la aumenta
 @param primes Contiene los primos encontrados
 @see enum_error.h
*/
enum error_t prime_container_memory_increase(struct prime_container * primes);

// procedure init ()
enum error_t prime_container_init(struct prime_container * primes) {
  assert(primes);
  enum error_t error = ERROR_SUCCESS;
  // size_used := 0
  primes->count = 0;
  // memory_size := 0
  primes->capacity = INITIAL_CAPACITY;
  // Hecho de manera generica ya que es para varios tipos de datos
  // container as array of desired data type
  primes->prime_numbers = (uint64_t*) calloc(
    primes->capacity, sizeof(uint64_t));
  if (primes->prime_numbers == NULL) {
    primes->capacity = 0;
    error = MEMORY_ERROR;
  }
  return error;
}
// end procedure

// procedure destroy ()
enum error_t prime_container_destroy(struct prime_container * primes) {
  assert(primes);
  // free memory in charge of container
  free(primes->prime_numbers);
  primes->count = 0;
  primes->capacity = 0;
  return ERROR_SUCCESS;
}
// end procedure

// procedure insert(data)
enum error_t prime_container_insert(
  struct prime_container * primes, uint64_t number) {
  assert(primes);
  enum error_t error = ERROR_SUCCESS;
  if (primes->count == primes->capacity) {
    // memory_increase()
    // containter[size_used-1] = data
    error = prime_container_memory_increase(primes);
  }
  // if (size_used <= memory_size)
  // Si hay espacio, inserte datos
  if (error == EXIT_SUCCESS) {
    // container[size_used-1] = data
    // Increment size_used by 1
    primes->prime_numbers[primes->count++] = number;
  }
  return error;
}
// end procedure

// procedure memory_increase()
enum error_t prime_container_memory_increase(struct prime_container * primes) {
  // Si es llamado, se ocupa más espacio
  assert(primes);
  enum error_t error = EXIT_SUCCESS;
  size_t new_capacity = INCREASE_FACTOR * primes->capacity;
  // container as a new array with 10 times more memory
  uint64_t * new_primes = (uint64_t*) calloc (new_capacity, sizeof(uint64_t));
  if (new_primes) {
    for (size_t i = 0; i < primes->count; ++i) {
      // Debe copiarse los datos
      // copy data from old to new container
      new_primes[i] = primes->prime_numbers[i];
    }
    // free old container
    free(primes->prime_numbers);
    primes->prime_numbers = new_primes;
    primes->capacity = new_capacity;
  } else {
    error = MEMORY_ERROR;
  }
  return error;
}
// end procedure

enum error_t calculate_primes(struct prime_container * primes, uint64_t limit) {
  assert(primes);
  enum error_t error = ERROR_SUCCESS;
  bool * sieve = (bool*) calloc(limit+1, sizeof(bool));
  if (sieve) {
    for (uint64_t i = 0; i < limit; ++i) {
      sieve[i] = false;
    }

    if (limit > 2) {
      sieve[2] = true;
    }

    if (limit > 3) {
      sieve[3] = true;
    }

    for (uint64_t x = 1; x * x <= limit; ++x) {
      for (uint64_t y = 1; y * y <= limit; ++y) {
        uint64_t n = (4 * x * x) + (y * y);
        if (n <= limit && (n % 12 == 1 || n % 12 == 5)) {
          sieve[n] ^= true;
        }

        n = (3 * x * x) + (y * y);
        if (n <= limit && n % 12 == 7) {
          sieve[n] ^= true;
        }

        n = (3 * x * x) - (y * y);
        if (x > y && n <= limit && n % 12 == 11) {
          sieve[n] ^= true;
        }
      }
    }

    for (uint64_t r = 5; r * r <= limit; r++) {
      if (sieve[r]) {
        for (uint64_t i = r * r; i <= limit; i += r * r) {
          sieve[i] = false;
        }
      }
    }

    for (uint64_t a = 1; a <= limit; a++) {
      if (sieve[a]) {
        error = prime_container_insert(primes, a);
      }
    }
  } else {
    error = MEMORY_ERROR;
  }

  free(sieve);
  return error;
}

void view_primes(struct prime_container * primes) {
    printf("[ ");
    for (size_t i = 0; i < primes->count; ++i) {
        if (i == primes->count - 1) {
          printf("%" PRIu64 "]\n", primes->prime_numbers[i]);
        } else {
          printf("%" PRIu64 " ,", primes->prime_numbers[i]);
        }
    }
}
