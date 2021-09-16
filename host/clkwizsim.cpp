#include "clkwiz.h"

int main(int argc, char **argv)
{
  float freq, freqmin, freqmax, freqstep;
  Clkwiz *clkwiz;

  if(argc < 4)
    errx(0, "usage: %s freqmin freqmax freqstep\n",
      argv[0]
    );
  int arg_channel = 1;
  freqmin = atof(argv[arg_channel++]);
  freqmax = atof(argv[arg_channel++]);
  freqstep = atof(argv[arg_channel++]);

  clkwiz = new Clkwiz(freqmin, freqmax, freqstep);

  for(clkwiz->restart(); !clkwiz->end(); clkwiz->next());
  freq = clkwiz->previous();

  std::cout << "clkwiz: from " << clkwiz->restart() << " to " << freq << " by step of " << freqstep << " (" << clkwiz->count() << " frequencies)\n";

  std::cout << "frequencies: ";
  for(freq = clkwiz->restart(); !clkwiz->end(); freq = clkwiz->next()) {
    std::cout << freq << ", ";
  }
  std::cout << '\n';

  return 0;
}
