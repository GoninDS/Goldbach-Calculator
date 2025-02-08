// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef SHARED_PRIVATE_DATA_H
#define SHARED_PRIVATE_DATA_H

#include <inttypes.h>
#include <pthread.h>

#include "numbers_array.h"
#include "prime_container.h"

struct shared_data {
  /**
  @brief Puntero al struct que contiene los números
  */
  struct numbers_array * numbers;
   /**
  @brief Puntero al struct que contiene los primos
  */
  struct prime_container * primes;
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

struct private_data {
  /**
  @brief Puntero para poder accesar el shared_data
  */
  struct shared_data * shared_data;
  /**
  @brief Variable que indica la primera posición que se va a procesar
  */
  uint64_t first_index;
};

#endif  // SHARED_PRIVATE_DATA_H
