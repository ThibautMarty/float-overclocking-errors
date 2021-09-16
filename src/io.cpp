#include "io.h"

void init_arrays
  ( data_t A[N][N]
  , data_t B[N][N]
#if OPERATION == OPMAC
  , data_t D[N][N]
#endif
  , std::mt19937& gen
  , int nexp
  )
{
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i][j] = randf(gen, nexp);
      B[i][j] = randf(gen, nexp);

#if OPERATION == OPMAC
      D[i][j] = randf(gen, nexp);
#endif
    }
  }
}

// TODO use data_t
void print_float
  ( float &in
  , std::stringstream &out
  )
{
  union {
    uint32_t i;
    float f;
  } u = { .f = in };
  out << in << ' '
    << ((u.i >> 31) & 1) << ' '     // sign
    << ((u.i >> 23) & 0xff) << ' '  // exponent
    << (u.i & 0x7fffff) << ' ';     // mantissa
}

int result_check
  ( data_t R[N][N]
  , data_t Rg[N][N]
  , std::stringstream *out
  )
{
  int r = 0;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {

      // NaN can happen if we sum +inf with -inf
      // In this case Rg != R is always true, even if both computed NaN
      // But this must not be considered as an error, so we enforce it
      if ((Rg[i][j] != R[i][j]) && !(isnan(Rg[i][j]) && isnan(R[i][j]))) {
        r++;

        if(out != nullptr) {
          *out << i << ' ' << j << ' ';
          print_float(Rg[i][j], *out);
          print_float(R[i][j], *out);
        }
      }
    }
  }

  return r;
}

data_t randf
  ( std::mt19937& gen
  , int nexp
  )
{
  union {
    unsigned long bytes;
    data_t f;
  } dual;

  constexpr int mb = std::numeric_limits<data_t>::digits - 1;
  constexpr int eb = sizeof(data_t) * 8 - mb - 1;
  constexpr int ebias = (1 << (eb - 1)) - 1;

  std::uniform_int_distribution<unsigned long> dist;
  unsigned long sign, exponent, mantissa;

  // TODO 1 << mb overflows for double
  decltype(dist.param()) param_sign(0, 1);
  decltype(dist.param()) param_mantissa(0, (1 << mb) - 1);
  // decltype(dist.param()) param_exponent(0, (1 << eb) - 2);
  decltype(dist.param()) param_exponent(
    std::max(ebias - (nexp - 1) / 2, 1),
    std::min(ebias + nexp / 2, (1 << eb) - 2)
  );

  dist.param(param_sign);
  sign = dist(gen);

  dist.param(param_mantissa);
  mantissa = dist(gen);

  dist.param(param_exponent);
  exponent = dist(gen);

  dual.bytes =
    (sign << (mb + eb)) |
    (exponent << mb) |
    mantissa;

  return dual.f;
}
