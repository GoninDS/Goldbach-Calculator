// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef PRIME_CONTAINER_H
#define PRIME_CONTAINER_H

#include <stdint.h>
#include <stdlib.h>

#include "enum_error.h"

struct prime_container {
  /**
    @brief Lleva la cuenta de elementos en el array
  */
  size_t count;
  /**
    @brief Lleva la cantidad de elementos posibles
  */
  size_t capacity;
  /**
    @brief Puntero que contiene los números primos
  */
  uint64_t * prime_numbers;
};

/**
  @brief Inicializa todos los valores en su forma más básica
  @param primes Puntero al struct en el que se quiere trabajar
  @see enum_error.h
*/
enum error_t prime_container_init(struct prime_container  * primes);

/**
  @brief Destruye el struct prime_container envíado como parámetro
  @param primes Puntero al struct en el que se quiere trabajar
  @see enum_error.h
*/
enum error_t prime_container_destroy(struct prime_container  * primes);

/**
  @brief Inserta un número en el array de números primos
  @param primes Puntero al struct en el que se quiere trabajar
  @param number Variable que contiene el número que se quiere insertar
  @see enum_error.h
*/
enum error_t prime_container_insert(
  struct prime_container  * primes, uint64_t number);

/**
  @brief Utiliza el Sieve of Atkin para calcular números primos
  @param primes Puntero al struct en el que se quiere trabajar
  @param limit Número más alto recibido
  @see enum_error.h
*/
enum error_t calculate_primes(struct prime_container  * primes, uint64_t limit);

/**
  @brief Muestra los números primos guardados
  @param primes Puntero al struct en el que se quiere trabajar
*/
void view_primes(struct prime_container  * primes);

#endif  // PRIME_CONTAINER_H
