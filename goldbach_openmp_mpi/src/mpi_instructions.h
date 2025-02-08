// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef MPI_INSTRUCTIONS_H
#define MPI_INSTRUCTIONS_H

#include <inttypes.h>

#include "numbers_array.h"

/**
 @brief Distribuye en diferentes bloques de trabajo para cada proceso
 @param numbers Contenedor que contiene los campos por ser distribuidos
 @param procedure_amount Cantidad de procesos para distribuir
 @see enum_error.h
 */
enum error_t distribute_numbers(struct numbers_array * numbers,
  uint64_t procedure_amount);

/**
 @brief Recibe los resultados de diversos procesos
 @param numbers Contenedor para guardar los resultados
 @see enum_error.h
 */
enum error_t receive_results(struct numbers_array * numbers);

/**
 @brief Envía los resultados al proceso 0
 @param numbers Contenedor con los resultados realizados por el proceso
 @param process_number Número del proceso actual
 @see enum_error.h
 */
enum error_t send_results(struct numbers_array * numbers,
  uint64_t process_number);

#endif  // MPI_INSTRUCTIONS_H
