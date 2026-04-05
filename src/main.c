#include "chudnovsky.h"
#include <mpfr.h>
#include <stdio.h>

// Constant terms
#define A 13591409
#define B 545140134
#define J 640320
#define D 426880
#define E 10005
#define C3 262537412640768000

void chudnovsky_term(mpfr_t term, int k);

int main() {
  int digits = 1000;

  mpfr_t pi;
  mpfr_init(pi);

  compute_pi(pi, digits);

  mpfr_printf("Pi = %.*Rf\n", digits, pi);

  mpfr_clear(pi);
  return 0;
}
