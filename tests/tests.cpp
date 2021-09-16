#include <gtest/gtest.h>
#include <stdlib.h>

#include "wrapper.h"
#include "golden.h"
#include "io.h"

namespace
{
  class MmultTest : public ::testing::Test
  {
    protected:
      data_t (*A)[N];
      data_t (*B)[N];
#if OPERATION == OPMAC
      data_t (*D)[N];
#endif
      data_t (*Rg)[N];
      data_t (*R)[N];

      virtual void SetUp()
      {
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

        init_arrays
          ( A
          , B
#if OPERATION == OPMAC
          , D
#endif
          , e2
          , 99999
          );
      }

      virtual void tearDown()
      {
        delete[] A;
        delete[] B;
#if OPERATION == OPMAC
        delete[] D;
#endif
        delete[] R;
        delete[] Rg;
      }
  };
} // namespace

TEST_F(MmultTest, ResultCheck)
{
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

  EXPECT_EQ(
    result_check(R, Rg),
    0
  );
}

TEST_F(MmultTest, InjectFault)
{
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
    , true
    );

  EXPECT_EQ(
    result_check(R, Rg),
    1
  );
}
