// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "mpi_instructions.h"

#include "assert.h"
#include "mpi.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// procedure distribute_numbers(numbers_array, procedure_amount)
enum error_t distribute_numbers(struct numbers_array * numbers,
  uint64_t procedure_amount) {
  assert(numbers);
  // number_amount := amount of numbers in numbers_array
  size_t number_amount = numbers->count;
  // Procedure amount is decreased because procedure 0
  // is not used for this purpose
  --procedure_amount;

  int64_t number = 0;
  uint64_t start = 0;
  uint64_t end = 0;
  bool can_continue = true;
  // for procedure := 0 to procedure_amount - 1 do
  for (uint64_t procedure = 0; procedure < procedure_amount; ++procedure) {
    // start := procedure * (div(number_amount, procedure_amount)) +
    // min (procedure, mod (number_amount, procedure_amount))
    start = procedure * (number_amount / procedure_amount) +
      MIN(procedure, number_amount % procedure_amount);
    // end := (procedure + 1) * (div(number_amount, procedure_amount)) +
    // min (procedure + 1, mod (number_amount, procedure_amount))
    end = (procedure + 1) * (number_amount / procedure_amount) +
      MIN(procedure + 1, number_amount % procedure_amount);
    can_continue = true;
    // for index := start to end - 1 do
    for (uint64_t index = start; index < end; ++index) {
      // Used to indicate that it needs another number
      // send true to procedure + 1
      if (MPI_Send(&can_continue, 1, MPI_C_BOOL, procedure + 1, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS) {
        return MPI_ERROR;
      }
      // Used to indicate where this number goes in the array
      // send index to procedure + 1
      if (MPI_Send(&index, 1, MPI_UINT64_T, procedure + 1, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS) {
        return MPI_ERROR;
      }
      // Used to send the number itself
      number = numbers->contained_numbers[index].number;
      if (numbers->contained_numbers[index].see_sums) {
        number = number * -1;
      }
      // send numbers_array[index] to procedure + 1
      if (MPI_Send(&number, 1, MPI_INT64_T, procedure + 1, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS) {
        return MPI_ERROR;
      }
    }
    can_continue = false;
    // No more numbers need to be received
    // send false to procedure + 1
    if (MPI_Send(&can_continue, 1, MPI_C_BOOL, procedure + 1, 0
      , MPI_COMM_WORLD) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // end for
  }
  // end for
  return ERROR_SUCCESS;
}
// end procedure

// procedure receive_results(numbers_array)
enum error_t receive_results(struct numbers_array * numbers) {
  assert(numbers);
  // number_amount := amount of numbers in numbers_array
  uint64_t number_amount = numbers->count;
  uint64_t process_number = 0;
  uint64_t sum_amount = 0;
  uint64_t count = 0;
  uint64_t * goldbach_sums;
  int64_t index = 0;
  // for counter := 0 to number_amount - 1 do
  for (uint64_t counter = 0; counter < number_amount; ++counter) {
    // Receive all results from any_procedure
    // receive procedure_number from any_procedure
    if (MPI_Recv(&process_number, 1, MPI_UINT64_T, MPI_ANY_SOURCE
      , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // receive index from procedure_number
    if (MPI_Recv(&index, 1, MPI_INT64_T, process_number
      , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)!= MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // receive sum_count from procedure_number
    if (MPI_Recv(&sum_amount, 1, MPI_UINT64_T, process_number
      , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // receive total_elements from procedure_number
    if (MPI_Recv(&count, 1, MPI_UINT64_T, process_number
      , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // receive results_array from procedure_number
    if (count > 0) {
      resize_sums(&numbers->contained_numbers[index], count);
      goldbach_sums = numbers->contained_numbers[index].goldbach_sums;
      if (MPI_Recv(&goldbach_sums[0], count, MPI_UINT64_T, process_number
        , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
        return MPI_ERROR;
      }
    }
    // move into numbers_array[index]
    numbers->contained_numbers[index].sum_amount = sum_amount;
    numbers->contained_numbers[index].count = count;

    if (numbers->contained_numbers[index].number < 4 ||
      numbers->contained_numbers[index].number == 5) {
      numbers->contained_numbers[index].is_valid = false;
    }
  }
  // end for
  return ERROR_SUCCESS;
}
// end procedure

// procedure send_results(numbers_array, process_number)
enum error_t send_results(struct numbers_array * numbers,
  uint64_t process_number) {
  assert(numbers);
  // number_amount := amount of numbers in numbers_array
  uint64_t number_amount = numbers->count;
  uint64_t sum_amount = 0;
  uint64_t count = 0;
  uint64_t * goldbach_sums;
  int64_t index = 0;
  // for counter := 0 to number_amount - 1 do
  for (uint64_t counter = 0; counter < number_amount; ++counter) {
    // Send all results from any_procedure to 0
    // send process_number to 0
    if (MPI_Send(&process_number, 1, MPI_UINT64_T, 0, 0
      , MPI_COMM_WORLD) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // send numbers_array[counter].index to 0
    index = numbers->contained_numbers[counter].index;
    if (MPI_Send(&index, 1, MPI_INT64_T, 0, 0
      , MPI_COMM_WORLD) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // send numbers_array[counter].sum_count to 0
    sum_amount = numbers->contained_numbers[counter].sum_amount;
    if (MPI_Send(&sum_amount, 1, MPI_UINT64_T, 0, 0
      , MPI_COMM_WORLD) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // send numbers_array[counter].count to 0
    count = numbers->contained_numbers[counter].count;
    if (MPI_Send(&count, 1, MPI_UINT64_T, 0, 0
      , MPI_COMM_WORLD) != MPI_SUCCESS) {
      return MPI_ERROR;
    }
    // send numbers_array[counter].sums to 0
    goldbach_sums = numbers->contained_numbers[counter].goldbach_sums;
    if (count > 0) {
      if (MPI_Send(&goldbach_sums[0], count, MPI_UINT64_T, 0, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS) {
        return MPI_ERROR;
      }
    }
  }
  // end for
  return ERROR_SUCCESS;
}
// end procedure
