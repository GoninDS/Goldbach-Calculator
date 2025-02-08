// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef GOLDBACH_THREAD_H
#define GOLDBACH_THREAD_H

#include "enum_error.h"
#include "numbers_array.h"
#include "prime_container.h"

/**
 @brief Decide de cuál manera se deben crear los threads
 @param thread_count Contiene la cantidad de threads
 @param numbers Puntero al struct que contiene los números
 @param primes Puntero al struct que contiene los primos requeridos
 @see enum_error.h
*/
enum error_t goldbach_thread_setup_threads(
  uint64_t thread_count,
    struct numbers_array * numbers, struct prime_container * primes);

#endif  // GOLDBACH_THREAD_H
