#ifndef __ACCELERATOR_H
#define __ACCELERATOR_H

#include "tools.h"
#include "options.h"

#define FAC TOSTRING(N / 2)

// Hardware toplevel function
#pragma SDS data access_pattern(A:SEQUENTIAL)
#pragma SDS data access_pattern(B:SEQUENTIAL)
#pragma SDS data access_pattern(R:SEQUENTIAL)
#pragma SDS data mem_attribute(A:PHYSICAL_CONTIGUOUS)
#pragma SDS data mem_attribute(B:PHYSICAL_CONTIGUOUS)
#pragma SDS data mem_attribute(R:PHYSICAL_CONTIGUOUS)
#if OPERATION == OPMAC
#pragma SDS data access_pattern(D:SEQUENTIAL)
#pragma SDS data mem_attribute(D:PHYSICAL_CONTIGUOUS)
#endif
void hw_toplevel
  ( data_t A[N][N]
  , data_t B[N][N]
  , data_t R[N][N]
#if OPERATION == OPMAC
  , data_t D[N][N]
#endif
#ifdef TESTS
  , bool injectfault
#endif
  );

#endif // __ACCELERATOR_H
