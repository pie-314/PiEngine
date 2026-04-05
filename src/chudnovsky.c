#include "chudnovsky.h"
#include <mpfr.h>

// Constant terms
#define A 13591409
#define B 545140134
#define J 640320
#define D 426880
#define E 10005
#define C3 262537412640768000

void chudnovsky_term(mpfr_t term, int k) {
  mpz_t six_k_fact, three_k_fact, k_fact;
  mpz_t numerator, denominator, temp;

  mpz_init(six_k_fact);
  mpz_init(three_k_fact);
  mpz_init(k_fact);
  mpz_init(numerator);
  mpz_init(denominator);
  mpz_init(temp);

  // (6k)!
  mpz_fac_ui(six_k_fact, 6 * k);

  // (3k)!
  mpz_fac_ui(three_k_fact, 3 * k);

  // (k)!
  mpz_fac_ui(k_fact, k);

  // numerator = (6k)! * (A + Bk)
  mpz_set_ui(temp, A + B * k);
  mpz_mul(numerator, six_k_fact, temp);

  // denominator = (3k)! * (k!)^3 * (C3)^k
  mpz_pow_ui(temp, k_fact, 3); // (k!)^3
  mpz_mul(denominator, three_k_fact, temp);

  mpz_ui_pow_ui(temp, C3, k); // (C3)^k
  mpz_mul(denominator, denominator, temp);

  // Convert to mpfr and divide
  mpfr_t num_f, den_f;
  mpfr_init(num_f);
  mpfr_init(den_f);

  mpfr_set_z(num_f, numerator, MPFR_RNDN);
  mpfr_set_z(den_f, denominator, MPFR_RNDN);

  mpfr_div(term, num_f, den_f, MPFR_RNDN);
  if (k % 2 != 0) {
    mpfr_neg(term, term, MPFR_RNDN);
  }

  // Cleanup
  mpz_clear(six_k_fact);
  mpz_clear(three_k_fact);
  mpz_clear(k_fact);
  mpz_clear(numerator);
  mpz_clear(denominator);
  mpz_clear(temp);
  mpfr_clear(num_f);
  mpfr_clear(den_f);
}
void set_precision(int digits) {
  int precision = digits * 3.321928 + 50;
  mpfr_set_default_prec(precision);
}

void compute_pi(mpfr_t pi, int digits) {
  int iterations = digits / 14 + 1;

  mpfr_t sum, term, C;
  mpfr_inits(sum, term, C, (mpfr_ptr)0);

  // Compute constant C
  compute_constant(C);

  mpfr_set_ui(sum, 0, MPFR_RNDN);

  for (int k = 0; k < iterations; k++) {
    chudnovsky_term(term, k);
    // mpfr_printf("term %d = %.50Rf\n", k, term);

    mpfr_add(sum, sum, term, MPFR_RNDN);
  }

  // pi = C / sum
  mpfr_div(pi, C, sum, MPFR_RNDN);

  // Clear memory
  mpfr_clears(sum, term, C, (mpfr_ptr)0);
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
