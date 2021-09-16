#ifndef __IO_H
#define __IO_H

#include "options.h"
#include <stdint.h>
#include <random>

void init_arrays
  ( data_t A[N][N]
  , data_t B[N][N]
#if OPERATION == OPMAC
  , data_t D[N][N]
#endif
  , std::mt19937& gen
  , int nexp
  );

void print_float
  ( float &in
  , std::stringstream &out
  );

int result_check
  ( data_t R[N][N]
  , data_t Rg[N][N]
  , std::stringstream *out = nullptr
  );

data_t randf
  (
  std::mt19937& gen,
  int nexp
  );

#endif // __IO_H
