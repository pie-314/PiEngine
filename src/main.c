#include "chudnovsky.h"
#include "cli.h"
#include <mpfr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constant terms
#define A 13591409
#define B 545140134
#define J 640320
#define D 426880
#define E 10005
#define C3 262537412640768000

void chudnovsky_term(mpfr_t term, int k);

int main(int argc, char *argv[]) {
  int digits = 1000;

  // Parse arguments (start from 1 to skip program name)
  for (int i = 1; i < argc; i++) {
    printf("argument %d is %s\n", i, argv[i]);

    // strcmp returns 0 when equal, so check for == 0
    if (strcmp(argv[i], "--digits") == 0 || strcmp(argv[i], "-d") == 0) {
      if (i + 1 < argc) {
        digits = atoi(argv[++i]); // Parse the next argument as integer
        printf("Input is a digit: %d\n", digits);
      } else {
        fprintf(stderr, "Error: --digits requires a value\n");
        return 1;
      }
    }
  }

  set_precision(digits);

  mpfr_t pi;
  mpfr_init(pi);

  compute_pi(pi, digits);

  mpfr_printf("Pi = %.*Rf\n", digits, pi);

  mpfr_clear(pi);
  return 0;
}
