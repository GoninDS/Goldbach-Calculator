// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef BOOLEAN_BIT_ARRAY
#define BOOLEAN_BIT_ARRAY

#include <stdbool.h>
#include <stdint.h>

#include "enum_error.h"

struct bit_array {
  /**
   @brief Arreglo para contener 8 bools en cada posición
  */
  uint8_t * sieve;
};

/**
  @brief Inicializa todos los valores en su forma más básica
  @param array Puntero al struct en el que se quiere trabajar
  @param size Entero con el tamaño deseado
  @see enum_error.h
*/
enum error_t bit_array_init(struct bit_array * array, uint64_t size);

/**
  @brief Destruyo el struct bit_array envíado como parámetro
  @param array Puntero al struct en el que se quiere trabajar
  @see enum_error.h
*/
enum error_t bit_array_destroy(struct bit_array * array);

/**
  @brief Ingresa un número como verdadero
  @param array Puntero al struct en el que se quiere trabajar
  @param number Número que se desea poner como verdadero
  @see enum_error.h
*/
enum error_t bit_array_set_true(struct bit_array * array, uint64_t number);

/**
  @brief Ingresa un número como falso
  @param array Puntero al struct en el que se quiere trabajar
  @param number Número que se desea poner como falso
  @see enum_error.h
*/
enum error_t bit_array_set_false(struct bit_array * array, uint64_t number);

/**
  @brief Retorna el valor de la posición
  @param array Puntero al struct en el que se quiere trabajar
  @param number Número que se desea obtener valor
*/
bool bit_array_get_value(struct bit_array * array, uint64_t number);

/**
  @brief Realiza un XOR con el valor negado y el actual
  @param array Puntero al struct en el que se quiere trabajar
  @param number Número que se desea realizar 
*/
bool bit_array_xor(struct bit_array * array, uint64_t number);

#endif  // BOOLEAN_BIT_ARRAY;
