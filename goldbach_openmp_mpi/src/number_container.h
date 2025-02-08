// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef NUMBER_CONTAINER_H
#define NUMBER_CONTAINER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "enum_error.h"

struct number_container {
  /**
    @brief Número cuyas sumas se quieren encontrar
  */
  int64_t number;
  /**
    @brief Cantidad de sumas de goldbach encontradas
  */
  uint64_t sum_amount;
  /**
    @brief Array dinámico que contiene las sumas de goldbach
  */
  uint64_t * goldbach_sums;
  /**
    @brief Contador de campos utilizados en el array dinámico
  */
  size_t count;
  /**
    @brief Capacidad de campos posibles en el array
  */
  size_t capacity;
  /**
    @brief Bool para detectar un número par
  */
  bool is_even;
  /**
    @brief Bool para detectar si un número es válido
  */
  bool is_valid;
  /**
    @brief Bool para detectar si se desea ver las sumas
  */
  bool see_sums;
  /**
    @brief Posición en el arreglo del procedure 0
  */
  int64_t index;
};

/**
 @brief Inicializa todos los valores en su forma más básica
 @param num_container Puntero al struct en el que se quiere trabajar
 @see enum_error.h
*/
enum error_t number_container_init(struct number_container * num_container);

/**
 @brief Recibe e inserta un número en el campo number del struct y lo procesa
 @param number Número que se quiere insertar en el struct
 @param num_container Puntero al struct en el que se quiere trabajar
 @see enum_error.h
*/
enum error_t number_container_set_number(
  struct number_container * num_container, int64_t number);

/**
 @brief Elimina el array dinámico de memoria
 @see number_container_init
 @see enum_error.h
*/
enum error_t number_container_destroy(struct number_container * num_container);

/**
 @brief Limpia los atributos del struct
 @see number_container_init
 @see enum_error.h
*/
enum error_t number_container_clear(struct number_container * num_container);

/**
 @brief Inserta un número en el array de sumas de goldbach
 @see number_container_init
 @see enum_error.h
*/
enum error_t number_container_insert(struct number_container * num_container,
  uint64_t number);

/**
 @brief Muestra los datos del número, sus sumas en caso de ser pedidas
 @see number_container_init
*/
void view_number(struct number_container * num_container);

/**
 @brief Resizes goldbach_sums
 @param num_container @see number_container_init
 @param count Size to resize to
*/
void resize_sums(struct number_container * num_container, uint64_t count);

#endif  // NUMBER_CONTAINER_H
