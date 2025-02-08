// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef GOLDBACH_CALCULATOR_H
#define GOLDBACH_CALCULATOR_H

#include "boolean_bit_array.h"
#include "enum_error.h"
#include "numbers_array.h"
#include "number_container.h"

/**
 @brief Recoge de la entrada estándar los valores de los números que se quieren
 @param array Struct para guardar los valores que se recogen
 @see enum_error.h
*/
enum error_t goldbach_calculator_obtain_numbers(struct numbers_array * array);

uint64_t goldbach_calculator_highest_number(struct numbers_array * array);

/**
 @brief Calcula las sumas de goldbach del número pedido
 @param number_container Struct para guardar el número
 @param sieve Struct para consultar los números primos
 @see enum_error.h
*/
enum error_t goldbach_calculator_calculate_number(
  struct number_container * number_container, struct bit_array * sieve);

/**
  @brief Calcula el array de bools en el Sieve of Atkin
  @param sieve Struct para guardar los números primos
  @param limit Número más alto recibido
  @see enum_error.h
*/
enum error_t calculate_sieve(struct bit_array * sieve,
  uint64_t limit);

#endif  // GOLDBACH_CALCULATOR_H
