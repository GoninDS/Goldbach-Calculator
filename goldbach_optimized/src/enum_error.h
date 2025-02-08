// Copyright 2022 Jeisson Hidalgo
// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#ifndef ENUM_ERROR_H
#define ENUM_ERROR_H

#include <stdlib.h>

enum error_t {
    /**
     @brief Se utiliza en caso de no haber error, equivale a 0
    */
    ERROR_SUCCESS = EXIT_SUCCESS,
    /**
     @brief Se utiliza en caso de haber error de lectura, equivale a 1
    */
    ERROR_READ_VALUES,
    /**
     @brief Se utiliza en caso de haber error de memoria, equivale a 2
    */
    MEMORY_ERROR,
    /**
     @brief Se utiliza en caso de haber error de acceso, equivale a 3
    */
    ILLEGAL_ACCESS,
    /**
      @brief Se utiliza en caso de no recibir bien los inputs, equivale a 4
    */
    INPUT_ERROR,
    /**
      @brief Se utiliza en caso de tener un error de threads, equivale a 5
    */
    THREAD_ERROR,
};

/**
 @brief Manda un mensaje indicando cuál error se encontró en caso de haber uno
 @param error Variable que contiene el error obtenido en ejecución
*/
void show_error(enum error_t error);

#endif  // ENUM_ERROR_H
