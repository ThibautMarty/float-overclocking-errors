#include <stdlib.h>

#include "wrapper.h"
#include "golden.h"
#include "io.h"


int main(int argc, char **argv)
{
  bool failed = false;
  std::stringstream out;

  data_t (*A)[N];
  data_t (*B)[N];
#if OPERATION == OPMAC
  data_t (*D)[N][N];
#endif
  data_t (*Rg)[N];
  data_t (*R)[N];

  A = new data_t[N][N];
  B = new data_t[N][N];
#if OPERATION == OPMAC
  D = new data_t[N][N];
#endif
  R = new data_t[N][N];
  Rg = new data_t[N][N];

  // Reproducible
  std::default_random_engine rd(42);
  std::mt19937 e2(rd());
  std::normal_distribution<data_t> dist(0.f, 100.f);

  init_arrays
    ( A
    , B
#if OPERATION == OPMAC
    , D
#endif
    , e2
    , dist
    );

  golden
    ( A
    , B
#if OPERATION == OPMAC
    , D
#endif
    , Rg
    );
  wrapper
    ( A
    , B
#if OPERATION == OPMAC
    , D
#endif
    , R
    );

  failed = result_check(R, Rg, &out) > 0;

  std::cerr << out.str() << '\n';

  delete[] A;
  delete[] B;
#if OPERATION == OPMAC
  delete[] D;
#endif
  delete[] R;
  delete[] Rg;

  return failed ? 1 : 0;
}
