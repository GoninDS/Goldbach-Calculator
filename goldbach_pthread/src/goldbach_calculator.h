// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef GOLDBACH_CALCULATOR_H
#define GOLDBACH_CALCULATOR_H

#include "enum_error.h"
#include "numbers_array.h"
#include "number_container.h"
#include "prime_container.h"

/**
 @brief Recoge de la entrada estándar los valores de los números que se quieren
 @param array Struct para guardar los valores que se recogen
 @see enum_error.h
*/
enum error_t goldbach_calculator_obtain_numbers(struct numbers_array * array);

/**
 @brief Prepara los números primos y los guarda en un contenedor de primos
 @param array Struct que contiene los números para el rango de primos
 @param primes Struct para guardar los números primos
 @see enum_error.h
*/
enum error_t goldbach_calculator_setup_primes(struct numbers_array * array,
  struct prime_container * primes);

/**
 @brief Consigue las sumas de goldbach de los números pedidos utilizando primos
 @see goldbach_calculator_setup_primes
 @see enum_error.h
*/
enum error_t goldbach_calculator_calculate_sums(struct numbers_array * array,
  struct prime_container * primes);

/**
 @brief Calcula las sumas de goldbach del número pedido
 @param number_container Struct para guardar el número
 @param primes Struct para guardar los números primos
 @see enum_error.h
*/
enum error_t goldbach_calculator_calculate_number(
  struct number_container * number_container, struct prime_container * primes);

#endif  // GOLDBACH_CALCULATOR_H
