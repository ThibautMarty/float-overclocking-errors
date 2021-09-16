#ifndef __WRAPPER_H
#define __WRAPPER_H

#include "sds_lib.h"
#include "accelerator.h"

// Software wrapper around accelerator, entry-point of the library, so
// that SDSoC know the caller of the accelerator
void wrapper
  ( data_t A[N][N]
  , data_t B[N][N]
#if OPERATION == OPMAC
  , data_t D[N][N]
#endif
  , data_t R[N][N]
#ifdef TESTS
  , bool injectfault = false
#endif
  );

#endif // __WRAPPER_H
