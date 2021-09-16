#ifndef __GOLDEN_H
#define __GOLDEN_H

#include "options.h"

void golden
  ( data_t A[N][N]
  , data_t B[N][N]
#if OPERATION == OPMAC
  , data_t D[N][N]
#endif
  , data_t R[N][N]
  );

#endif // __GOLDEN_H
