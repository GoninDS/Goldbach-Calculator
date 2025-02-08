// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef NUMBERS_ARRAY_H
#define NUMBERS_ARRAY_H

#include <stdint.h>
#include <stdlib.h>

#include "enum_error.h"
#include "number_container.h"

struct numbers_array {
  /**
  @brief Lleva la cuenta de elementos en el array
  */
  size_t count;
  /**
  @brief Lleva la cantidad de posibles elementos
  */
  size_t capacity;
  /**
  @brief Contiene los elementos
  */
  struct number_container * contained_numbers;
};

/**
 @brief Inicializa el struct de numbers_array
 @param numbers El struct que quiere ser modificado
 @see enum_error.h
*/
enum error_t numbers_array_init(struct numbers_array * numbers);

/**
 @brief Destruye el array dinámico dentro del struct
 @see numbers_array_init
 @see enum_error.h
*/
enum error_t numbers_array_destroy(struct numbers_array * numbers);

/**
 @brief Inserta en el array dinámico
 @param numbers El struct que contiene el array
 @param number El número que se desea insertar
 @param index La posición en el arreglo principal
 @see enum_error.h
*/
enum error_t numbers_array_insert(struct numbers_array * numbers,
  int64_t number, int64_t index);

/**
 @brief Muestra la información de todos los números contenidos en el struct
 @see numbers_array_init
*/
void view_numbers(struct numbers_array * numbers);

#endif  // NUMBERS_ARRAY_H
