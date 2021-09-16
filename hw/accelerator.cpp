#include "accelerator.h"

void hw_dotproduct
  ( data_t a[N]
  , data_t b[N]
  , data_t *c
  )
{
  for(int i = 0; i < N; i++) {
#pragma HLS PIPELINE
    data_t prod = a[i] * b[i];

#ifdef NODSP
#pragma HLS RESOURCE variable=prod core=DMul_nodsp
#pragma HLS RESOURCE variable=prod core=FMul_nodsp
#pragma HLS RESOURCE variable=prod core=HMul_nodsp
#else
#pragma HLS RESOURCE variable=prod core=DMul_fulldsp
#pragma HLS RESOURCE variable=prod core=FMul_fulldsp
#pragma HLS RESOURCE variable=prod core=HMul_fulldsp
#endif

    if(i == 0)
      *c = prod;
    else {
      data_t sum;

#ifdef NODSP
#pragma HLS RESOURCE variable=sum core=DAddSub_nodsp
#pragma HLS RESOURCE variable=sum core=FAddSub_nodsp
#pragma HLS RESOURCE variable=sum core=HAddSub_nodsp
#else
#pragma HLS RESOURCE variable=sum core=DAddSub_fulldsp
#pragma HLS RESOURCE variable=sum core=FAddSub_fulldsp
#pragma HLS RESOURCE variable=sum core=HAddSub_fulldsp
#endif

      sum = *c + prod;
      *c = sum;
    }
  }
}

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
  )
{
  static data_t _A[N][N], _B[N][N];
  static data_t _R[N][N];
#if OPERATION == OPMAC
  static data_t _D[N][N];
#endif

/*
#pragma HLS RESOURCE variable=_A core=RAM_2P_BRAM
#pragma HLS RESOURCE variable=_B core=RAM_2P_BRAM
#pragma HLS RESOURCE variable=_R core=RAM_2P_BRAM
*/

DO_PRAGMA(HLS array_partition variable=_A cyclic factor=FAC dim=2)
DO_PRAGMA(HLS array_partition variable=_B cyclic factor=FAC dim=2)

#if OPERATION == OPMAC
DO_PRAGMA(HLS array_partition variable=_D cyclic factor=FAC dim=2)
#endif

#if OPERATION != OPMUL
DO_PRAGMA(HLS array_partition variable=_R cyclic factor=FAC dim=2)
#endif // OPMUL

#pragma HLS dataflow

#if OPERATION == OPMUL
  hw_recv_matrix_transpose<N>(B, _B);
#else // OPMUL
  hw_recv_matrix<N>(B, _B);
#endif // OPMUL

  hw_recv_matrix<N>(A, _A);

#if OPERATION == OPMAC
  hw_recv_matrix<N>(D, _D);
#endif

  compute:for(int row = 0; row < N; row++) {
#if OPERATION != OPMUL
#pragma HLS PIPELINE
#endif // OPMUL

    for(int col = 0; col < N; col++) {
#pragma HLS PIPELINE

#if OPERATION == OPMUL

      hw_dotproduct
        ( _A[row]
        , _B[col]
        , &_R[row][col]
        );

#elif OPERATION == OPEWMUL

      data_t prod;

#ifdef NODSP
#pragma HLS RESOURCE variable=prod core=DMul_nodsp
#pragma HLS RESOURCE variable=prod core=FMul_nodsp
#pragma HLS RESOURCE variable=prod core=HMul_nodsp
#else
#pragma HLS RESOURCE variable=prod core=DMul_fulldsp
#pragma HLS RESOURCE variable=prod core=FMul_fulldsp
#pragma HLS RESOURCE variable=prod core=HMul_fulldsp
#endif

      prod = _A[row][col] * _B[row][col];
      _R[row][col] = prod;

#elif OPERATION == OPADD

      data_t sum;

#ifdef NODSP
#pragma HLS RESOURCE variable=sum core=DAddSub_nodsp
#pragma HLS RESOURCE variable=sum core=FAddSub_nodsp
#pragma HLS RESOURCE variable=sum core=HAddSub_nodsp
#else
#pragma HLS RESOURCE variable=sum core=DAddSub_fulldsp
#pragma HLS RESOURCE variable=sum core=FAddSub_fulldsp
#pragma HLS RESOURCE variable=sum core=HAddSub_fulldsp
#endif // NODSP

      sum = _A[row][col] + _B[row][col];
      _R[row][col] = sum;

#elif OPERATION == OPMAC

      data_t prod, sum;
#ifdef NODSP
#pragma HLS RESOURCE variable=sum core=DAddSub_nodsp
#pragma HLS RESOURCE variable=sum core=FAddSub_nodsp
#pragma HLS RESOURCE variable=sum core=HAddSub_nodsp
#pragma HLS RESOURCE variable=prod core=DMul_nodsp
#pragma HLS RESOURCE variable=prod core=FMul_nodsp
#pragma HLS RESOURCE variable=prod core=HMul_nodsp
#else
#pragma HLS RESOURCE variable=sum core=DAddSub_fulldsp
#pragma HLS RESOURCE variable=sum core=FAddSub_fulldsp
#pragma HLS RESOURCE variable=sum core=HAddSub_fulldsp
#pragma HLS RESOURCE variable=prod core=DMul_fulldsp
#pragma HLS RESOURCE variable=prod core=FMul_fulldsp
#pragma HLS RESOURCE variable=prod core=HMul_fulldsp
#endif // NODSP

      prod = _A[row][col] * _B[row][col];
      sum = prod + _D[row][col];
      _R[row][col] = sum;

#endif // OPERATION

#ifdef TESTS
      if(injectfault && (row == 0) && (col == 0))
        _R[row][col] = (_R[row][col] == 0) ? 1 : 0;
#endif // TESTS

    }
  }

  hw_send_matrix<N>(_R, R);
}
