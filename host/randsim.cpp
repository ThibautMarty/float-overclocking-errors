#include <err.h>
#include "options.h"
#include "io.h"

#define NTESTS 10000

int main(int argc, char **argv)
{
  data_t d;
  int nexp;

  if(argc < 2)
    errx(0, "usage: %s nexp\n",
      argv[0]
    );
  int arg_channel = 1;
  nexp = atoi(argv[arg_channel++]);

  // Reproducible
  std::default_random_engine rd(42);
  std::mt19937 gen(rd());

  for(int i = 0; i < NTESTS; i++) {
    std::stringstream out;
    d = randf(gen, nexp);
    print_float(d, out);
    std::cout << out.str() << '\n';
  }

  return 0;
}
