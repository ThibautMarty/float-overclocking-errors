#include "golden.h"

void golden
  ( data_t A[N][N]
  , data_t B[N][N]
#if OPERATION == OPMAC
  , data_t D[N][N]
#endif
  , data_t R[N][N]
  )
{
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {

#if OPERATION == OPMUL

      data_t result = 0.0;
      for (int k = 0; k < N; k++) {
        result += A[row][k] * B[k][col];
      }
      R[row][col] = result;

#elif OPERATION == OPEWMUL

      R[row][col] = A[row][col] * B[row][col];

#elif OPERATION == OPADD

      R[row][col] = A[row][col] + B[row][col];

#elif OPERATION == OPMAC

      R[row][col] = A[row][col] * B[row][col] + D[row][col];

#endif // OPERATION
    }
  }
}
