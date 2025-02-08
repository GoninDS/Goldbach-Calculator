// Copyright 2022 Luis Solano <luis.solanosantamaria@ucr.ac.cr>

#include "enum_error.h"

#include <stdio.h>

void show_error(enum error_t error) {
  switch (error) {
    case ERROR_SUCCESS:
      // No error to show
    case ERROR_READ_VALUES:
      fprintf(stderr, "Error: could not properly read values\n");
      break;
    case MEMORY_ERROR:
      fprintf(stderr, "Error: could not allocate memory\n");
      break;
    case ILLEGAL_ACCESS:
      fprintf(stderr, "Error: illegal memory acess\n");
      break;
    case INPUT_ERROR:
      fprintf(stderr, "Error: did not receive correct parameters\n");
      break;
    case THREAD_ERROR:
      fprintf(stderr, "Error: could not properly create threads\n");
      break;
  }
}
