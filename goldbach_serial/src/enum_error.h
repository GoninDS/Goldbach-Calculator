// Copyright 2022 Jeisson Hidalgo
// Implemented by Luis Solano

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
     @brief Se utiliza en caso de haber error de acceso, equivale a 2
    */
    ILLEGAL_ACCESS,
};

#endif  // ENUM_ERROR_H
