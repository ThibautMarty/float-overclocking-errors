#include "wrapper.h"

void wrapper
  ( data_t A[N][N]
  , data_t B[N][N]
#if OPERATION == OPMAC
  , data_t D[N][N]
#endif
  , data_t R[N][N]
#ifdef TESTS
  , bool injectfault
#endif
  )
{
  hw_toplevel
    ( A
    , B
#if OPERATION == OPMAC
    , D
#endif
    , R
#ifdef TESTS
    , injectfault
#endif
    );
}
