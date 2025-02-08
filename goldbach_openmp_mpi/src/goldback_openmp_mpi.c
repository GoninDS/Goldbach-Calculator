// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
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
#include "mpi_instructions.h"

// procedure main (thread_count)
int main(int argc, char * argv[]) {
  enum error_t error = ERROR_SUCCESS;

  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;
    // read process_number
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    // Get thread count
    uint64_t thread_count = omp_get_max_threads();
    // If inserted as param, update thread count
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
      if (process_number != 0) {
        // bool can_receive := true
        bool can_receive = true;
        // while (receive can_receive from procedure 0) do
        uint64_t index = 0;
        int64_t number = 0;
        while (can_receive && error == ERROR_SUCCESS) {
          if (MPI_Recv(&can_receive, 1, MPI_C_BOOL, 0,
            0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
            error = MPI_ERROR;
            break;
          }
          if (can_receive) {
            // receive index from procedure 0
            if (MPI_Recv(&index, 1, MPI_UINT64_T, 0,
              0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
              error = MPI_ERROR;
              break;
            }
            // receive number from procedure 0
            if (MPI_Recv(&number, 1, MPI_INT64_T, 0,
              0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
              error = MPI_ERROR;
              break;
            }
            // insert number with index in numbers_array
            error = numbers_array_insert(&array, number, index);
          }
        }
        if (error == ERROR_SUCCESS) {
          // end while
          uint64_t highest_number =
            goldbach_calculator_highest_number(&array);
          // prime_container as a dynamic boolean bit array
          struct bit_array sieve_container;
          uint64_t array_size = highest_number / 8;
          ++array_size;
          error = bit_array_init(&sieve_container, array_size);
          if (error == ERROR_SUCCESS) {
            // setup_primes(numbers_array, prime_container)
            error = calculate_sieve(&sieve_container, highest_number + 1);
            if (error == ERROR_SUCCESS) {
              if (sieve_container.sieve) {
                // setup_threads(thread_count, numbers_array, prime_container)
                error = goldbach_thread_setup_threads(thread_count, &array,
                  &sieve_container);
              }
              // delete prime_container
              bit_array_destroy(&sieve_container);
            }
          }
          if (error == ERROR_SUCCESS) {
            // send_results(numbers_array, process_number)
            send_results(&array, process_number);
          }
        }
        numbers_array_destroy(&array);
      } else {
        // if process_number == 0 do
        // obtain_numbers(numbers_array)
        error = goldbach_calculator_obtain_numbers(&array);
        if (error == ERROR_SUCCESS) {
          // read procedure_amount
          int process_count = 0;
          if (MPI_Comm_size(MPI_COMM_WORLD, &process_count) != MPI_SUCCESS) {
            error = MPI_ERROR;
          }
          if (process_count == 1 && error == ERROR_SUCCESS) {
            uint64_t highest_number =
              goldbach_calculator_highest_number(&array);
            struct bit_array sieve_container;
            uint64_t array_size = highest_number / 8;
            ++array_size;
            error = bit_array_init(&sieve_container, array_size);
            if (error == ERROR_SUCCESS) {
              error = calculate_sieve(&sieve_container, highest_number + 1);
              if (error == ERROR_SUCCESS) {
                if (sieve_container.sieve) {
                  // setup_threads(thread_count, numbers_array, primes_array)
                  error = goldbach_thread_setup_threads(thread_count, &array,
                    &sieve_container);
                  if (error == ERROR_SUCCESS) {
                    view_numbers(&array);
                  }
                }
                bit_array_destroy(&sieve_container);
              }
            }
          } else {
            if (error == ERROR_SUCCESS) {
              // distribute_numbers(numbers_array, procedure_amount)
              error = distribute_numbers(&array, process_count);
              // receive_results(numbers_array)
              if (error == ERROR_SUCCESS) {
                error = receive_results(&array);
                // show_results(numbers_array)
                if (error == ERROR_SUCCESS) {
                  view_numbers(&array);
                }
              }
            }
          }
          // end if
        }
        numbers_array_destroy(&array);
      }
    }
    show_error(error);
    MPI_Finalize();
  } else {
    show_error(MPI_ERROR);
  }

  return error;
}
// end procedure
