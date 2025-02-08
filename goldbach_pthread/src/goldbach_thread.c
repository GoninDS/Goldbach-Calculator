// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "goldbach_thread.h"

#include <assert.h>
#include <pthread.h>

#include <stdio.h>

#include "data.h"
#include "goldbach_calculator.h"

/**
 @brief Crea los threads en caso de haber más números que threads
 @param shared_data Puntero al struct que contiene los datos compartidos
 @see enum_error.h
*/
enum error_t goldbach_thread_less_threads(struct shared_data * shared_data);

/**
 @brief Crea los threads en caso de haber igual o mayor cantidad de threads
 @param shared_data Puntero al struct que contiene los datos compartidos
 @param count 
 @see enum_error.h
*/
enum error_t goldbach_thread_normal_threads(struct shared_data * shared_data,
  uint64_t count);

/**
 @brief Proceso de los threads en caso de haber más números que threads
 @param data Contiene el shared_data de manera void *
 @return NULL
*/
void * run_less(void * data);

/**
 @brief Proceso de los threads en caso de tener >= cantidad de threads
 @param data Contiene el shared_data de manera void *
 @return NULL
*/
void * run_equal(void * data);

enum error_t goldbach_thread_setup_threads(uint64_t thread_count,
  struct numbers_array * numbers, struct prime_container * primes) {
  // create shared_data for threads
  struct shared_data shared_data;
  // save numbers_array in shared_data
  shared_data.numbers = numbers;
  // save primes_array in shared_data
  shared_data.primes = primes;

  shared_data.thread_count = thread_count;
  shared_data.processed_numbers = 0;

  enum error_t error = ERROR_SUCCESS;
  // if (thread_count < numbers_array size)
  if (thread_count < numbers->count) {
    // less_threads(thread_count, shared_data)
    error = goldbach_thread_less_threads(&shared_data);
  } else {
    // if (thread_count > numbers_array size)
    if (thread_count > numbers->count) {
      // more_threads(thread_count, shared_data)
      error = goldbach_thread_normal_threads(&shared_data, numbers->count);
    } else {
      // equal_threads(thread_count, shared_data)
      error = goldbach_thread_normal_threads(&shared_data, thread_count);
    }
  }
  return error;
}

enum error_t goldbach_thread_less_threads(struct shared_data * shared_data) {
  assert(shared_data);
  enum error_t error = ERROR_SUCCESS;

  pthread_t * threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));

  if (threads) {
    struct private_data * private_data = (struct private_data *)
      calloc(shared_data->thread_count, sizeof(struct private_data));
    if (private_data) {
      // for index := 0 to thread_count do

      // Se inicializa el mutex
      error = pthread_mutex_init(&shared_data->can_access_index, NULL);
      shared_data->processed_numbers = shared_data->thread_count - 1;

      for (uint64_t index = 0; index < shared_data->thread_count; ++index) {
        private_data[index].first_index = index;
        private_data[index].shared_data = shared_data;
        // create_thread(calculate_equal, shared_data, index)
        error = pthread_create
          (&threads[index], NULL, run_less, &private_data[index]);
      // end for
        if (error == ERROR_SUCCESS) {
        } else {
          error = THREAD_ERROR;
          break;
        }
      }

      for (uint64_t index = 0; index < shared_data->thread_count; ++index) {
        pthread_join(threads[index], NULL);
      }

      pthread_mutex_destroy(&shared_data->can_access_index);

      free(private_data);
    } else {
      error = MEMORY_ERROR;
    }
    free(threads);
  } else {
    error = MEMORY_ERROR;
  }

  return error;
}

// procedure equal_threads(thread_count, shared_data)
// procedure more_threads(thread_count, shared_data)
enum error_t goldbach_thread_normal_threads(struct shared_data * shared_data,
  uint64_t count) {
  assert(shared_data);
  enum error_t error = ERROR_SUCCESS;

  pthread_t * threads = (pthread_t*)
    malloc(count * sizeof(pthread_t));

  if (threads) {
    struct private_data * private_data = (struct private_data *)
      calloc(count, sizeof(struct private_data));
    if (private_data) {
      // for index := 0 to thread_count do
      // for index := 0 to shared_data numbers_array size do
      for (uint64_t index = 0; index < count; ++index) {
        private_data[index].first_index = index;
        private_data[index].shared_data = shared_data;
        // create_thread(calculate_equal, shared_data, index)
        error = pthread_create
          (&threads[index], NULL, run_equal, &private_data[index]);
      // end for
        if (error == ERROR_SUCCESS) {
        } else {
          error = THREAD_ERROR;
          break;
        }
      }
      for (uint64_t index = 0; index < count; ++index) {
        pthread_join(threads[index], NULL);
      }
      free(private_data);
    } else {
      error = MEMORY_ERROR;
    }
    free(threads);
  } else {
    error = MEMORY_ERROR;
  }

  return error;
}

// procedure equal(shared_data, my_index)
void * run_equal(void * data) {
  assert(data);
  struct private_data * private_data = (struct private_data *) data;
  struct shared_data * shared_data = private_data->shared_data;

  struct numbers_array * numbers = shared_data->numbers;
  struct prime_container * primes = shared_data->primes;
  uint64_t my_index = private_data->first_index;

  // calculate_number(shared_data[my_index])
  goldbach_calculator_calculate_number(
    &(numbers->contained_numbers[my_index]), primes);
  return NULL;
}
// end procedure

// procedure less(shared_data, first_index)
void * run_less(void * data) {
  assert(data);
  struct private_data * private_data = (struct private_data *) data;
  struct shared_data * shared_data = private_data->shared_data;

  struct numbers_array * numbers = shared_data->numbers;
  struct prime_container * primes = shared_data->primes;
  uint64_t my_index = private_data->first_index;

  // while true do
  while (true) {
    // calculate_number(shared_data[my_index])
    goldbach_calculator_calculate_number(
      &(numbers->contained_numbers[my_index]), primes);

    // lock
    pthread_mutex_lock(&shared_data->can_access_index);
      // if (processed_numbers < shared_data number_array size)
      if (shared_data->processed_numbers < shared_data->numbers->count) {
        // ++processed_numbers
        shared_data->processed_numbers = 1 + shared_data->processed_numbers;
        // my_index = processed_numbers
        my_index = shared_data->processed_numbers;
      } else {
        pthread_mutex_unlock(&shared_data->can_access_index);
        break;
      }
    // unlock
    pthread_mutex_unlock(&shared_data->can_access_index);
  }
  return NULL;
}
// end procedure
