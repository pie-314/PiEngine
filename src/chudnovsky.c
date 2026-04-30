#include "chudnovsky.h"
#include <mpfr.h>

// Constant terms
#define A 13591409
#define B 545140134
#define J 640320
#define D 426880
#define E 10005
#define C3 262537412640768000

void set_precision(int digits) {
  int precision = digits * 3.321928 + 10000;
  mpfr_set_default_prec(precision);
}

void compute_pi(mpfr_t pi, int digits) {
  int iterations = digits / 14 + 2;

  mpfr_t sum, term, C;
  mpfr_t num, den, temp, ratio1, ratio2;

  mpfr_inits(sum, term, C, num, den, temp, ratio1, ratio2, (mpfr_ptr)0);

  // C = 426880 * rt(10005)
  compute_constant(C);

  mpfr_set_ui(term, A, MPFR_RNDN);
  mpfr_set(sum, term, MPFR_RNDN);

  for (int k = 1; k < iterations; k++) {

    // num = (6k-5)(6k-4)...(6k)
    mpfr_set_ui(num, 6 * k - 5, MPFR_RNDN);
    for (int i = 1; i < 6; i++) {
      mpfr_mul_ui(num, num, 6 * k - 5 + i, MPFR_RNDN);
    }

    // den = k^3
    mpfr_set_ui(den, k, MPFR_RNDN);
    mpfr_pow_ui(den, den, 3, MPFR_RNDN);

    // multiply (3k-2)(3k-1)(3k)
    mpfr_set_ui(temp, 3 * k - 2, MPFR_RNDN);
    mpfr_mul_ui(temp, temp, 3 * k - 1, MPFR_RNDN);
    mpfr_mul_ui(temp, temp, 3 * k, MPFR_RNDN);
    mpfr_mul(den, den, temp, MPFR_RNDN);

    // multiply by C3
    mpfr_mul_ui(den, den, C3, MPFR_RNDN);

    // ratio1 = num / den
    mpfr_div(ratio1, num, den, MPFR_RNDN);

    // ratio2 = (A + Bk) / (A + B(k-1))
    mpfr_set_ui(ratio2, B, MPFR_RNDN);
    mpfr_mul_ui(ratio2, ratio2, k, MPFR_RNDN);
    mpfr_add_ui(ratio2, ratio2, A, MPFR_RNDN);

    mpfr_set_ui(temp, B, MPFR_RNDN);
    mpfr_mul_ui(temp, temp, k - 1, MPFR_RNDN);
    mpfr_add_ui(temp, temp, A, MPFR_RNDN);

    mpfr_div(ratio2, ratio2, temp, MPFR_RNDN);

    // term = term * ratio1 * ratio2
    mpfr_mul(term, term, ratio1, MPFR_RNDN);
    mpfr_mul(term, term, ratio2, MPFR_RNDN);

    mpfr_neg(term, term, MPFR_RNDN);

    mpfr_add(sum, sum, term, MPFR_RNDN);
  }

  // pi = C / sum
  mpfr_div(pi, C, sum, MPFR_RNDN);

  mpfr_clears(sum, term, C, num, den, temp, ratio1, ratio2, (mpfr_ptr)0);
}

void compute_constant(mpfr_t C) {
  mpfr_t sqrtC;
  mpfr_init(sqrtC);

  // sqrtC = sqrt(10005)
  mpfr_set_ui(sqrtC, E, MPFR_RNDN);
  mpfr_sqrt(sqrtC, sqrtC, MPFR_RNDN);

  // C = 426880 * sqrt(10005)
  mpfr_set_ui(C, D, MPFR_RNDN);
  mpfr_mul(C, C, sqrtC, MPFR_RNDN);

  mpfr_clear(sqrtC);
}
