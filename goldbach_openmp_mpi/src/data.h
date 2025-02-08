// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <stdbool.h>
#include <inttypes.h>
#include <pthread.h>

#include "boolean_bit_array.h"
#include "numbers_array.h"

struct shared_data {
  /**
  @brief Puntero al struct que contiene los números
  */
  struct numbers_array * numbers;
   /**
  @brief Arreglo con primos
  */
  struct bit_array * sieve;
   /**
  @brief Variable que lleva la posición del array en la que se va a trabajar
  */
  uint64_t processed_numbers;
  /**
  @brief Variable que lleva la cantidad de threads
  */
  uint64_t thread_count;
  /**
  @brief Mutex para accesar de manera segura processed_numbers
  */
  pthread_mutex_t can_access_index;
};

#endif  // SHARED_DATA_H
