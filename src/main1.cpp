#include <iostream>
#include "wrapper.h"
#include "golden.h"
#include "io.h"
#include "clkwiz.h"

int main(int argc, char **argv)
{
  data_t (*A)[N][N];
  data_t (*B)[N][N];
#if OPERATION == OPMAC
  data_t (*D)[N][N];
#endif
  data_t (*Rg)[N][N];
  data_t (*R)[N];
  Clkwiz *clkwiz;
  int img, imgs, errors;
  float freq, freqmin, freqmax, freqstep;
  int nexp;
  bool quiet;

  if(argc < 6)
    errx(0, "usage: %s nbimgs freqmin freqmax freqstep nexp [quiet]\n",
      argv[0]
    );
  int arg_channel = 1;
  imgs = atoi(argv[arg_channel++]);
  freqmin = atof(argv[arg_channel++]);
  freqmax = atof(argv[arg_channel++]);
  freqstep = atof(argv[arg_channel++]);
  nexp = atoi(argv[arg_channel++]);
  quiet = argc > 6;

  A =  (data_t (*)[N][N]) sds_alloc(imgs * N * N * sizeof(data_t));
  B =  (data_t (*)[N][N]) sds_alloc(imgs * N * N * sizeof(data_t));
#if OPERATION == OPMAC
  D =  (data_t (*)[N][N]) sds_alloc(imgs * N * N * sizeof(data_t));
#endif
  Rg = (data_t (*)[N][N]) sds_alloc(imgs * N * N * sizeof(data_t));
  R =  (data_t (*)[N]) sds_alloc(N * N * sizeof(data_t));

  if (!A || !B || !R || !Rg) {
    if (A) sds_free(A);
    if (B) sds_free(B);
#if OPERATION == OPMAC
    if (D) sds_free(D);
#endif
    if (Rg) sds_free(Rg);
    if (R) sds_free(R);
    return 2;
  }

  // Reproducible
  std::default_random_engine rd(42);
  std::mt19937 gen(rd());

  // Prepare images
  for(img = 0; img < imgs; img++) {
    init_arrays
      ( A[img]
      , B[img]
#if OPERATION == OPMAC
      , D[img]
#endif
      , gen
      , nexp
      );
    golden
      ( A[img]
      , B[img]
#if OPERATION == OPMAC
      , D[img]
#endif
      , Rg[img]
      );
  }

  clkwiz = new Clkwiz(freqmin, freqmax, freqstep);

  for(img = 0; img < imgs; img++)
  {
    for(freq = clkwiz->restart(); !clkwiz->end(); freq = clkwiz->next())
    {
      std::stringstream out;

      std::cerr <<
        nexp << ' ' <<
        img << ' ' <<
        imgs << ' ' <<
        freq << ' ';

      std::cout <<
        img << ' ' <<
        imgs << ' ' <<
        freq << ' ';

      wrapper
        ( A[img]
        , B[img]
#if OPERATION == OPMAC
        , D[img]
#endif
        , R
        );

      if(quiet) {
        errors = result_check(R, Rg[img]);
        std::cerr << errors << '\n';
        std::cout << errors << '\n';
      }
      else {
        errors = result_check(R, Rg[img], &out);
        std::cerr << errors << '\n';
        std::cout << errors << ' ' << out.str() << '\n';
      }
    }
  }

  delete clkwiz;

  sds_free(A);
  sds_free(B);
#if OPERATION == OPMAC
  sds_free(D);
#endif
  sds_free(R);
  sds_free(Rg);

  return 0;
}
