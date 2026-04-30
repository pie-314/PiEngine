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

void compute_bs(long a, long b, mpz_t P, mpz_t Q, mpz_t T) {
  if (b - a == 1) {
    // case of k = 0
    if (a == 0) {
      mpz_set_ui(P, 1);
      mpz_set_ui(Q, 1);
      mpz_set_ui(T, A);
    } else {

      mpz_t p, q, t, tmp;
      mpz_inits(p, q, t, tmp, NULL);

      // P = (6k-5)(2k-1)(6k-1)
      mpz_set_ui(p, 6 * a - 5);
      mpz_mul_ui(p, p, 2 * a - 1);
      mpz_mul_ui(p, p, 6 * a - 1);

      // Q = k^3 * C3
      mpz_set_ui(q, a);
      mpz_pow_ui(q, q, 3);
      mpz_mul_ui(q, q, C3);

      // T = P * (A + Bk)
      mpz_set_ui(tmp, B);
      mpz_mul_ui(tmp, tmp, a);
      mpz_add_ui(tmp, tmp, A);

      mpz_mul(t, p, tmp);

      // Alternate sign
      if (a % 2 != 0)
        mpz_neg(t, t);

      // Assign results to output variables
      mpz_set(P, p);
      mpz_set(Q, q);
      mpz_set(T, t);

      // Free temporary variables
      mpz_clears(p, q, t, tmp, NULL);
    }

  } else {
    long m = (a + b) / 2;

    mpz_t P1, Q1, T1;
    mpz_t P2, Q2, T2;

    mpz_inits(P1, Q1, T1, P2, Q2, T2, NULL);

    compute_bs(a, m, P1, Q1, T1);
    compute_bs(m, b, P2, Q2, T2);

    mpz_mul(P, P1, P2);
    mpz_mul(Q, Q1, Q2);

    mpz_t temp1, temp2;
    mpz_inits(temp1, temp2, NULL);

    mpz_mul(temp1, T1, Q2);
    mpz_mul(temp2, P1, T2);
    mpz_add(T, temp1, temp2);

    mpz_clears(P1, Q1, T1, P2, Q2, T2, temp1, temp2, NULL);
  }
}

void compute_pi(mpfr_t pi, int digits) {

  int iterations = digits / 14 + 2;

  mpz_t P, Q, T;
  mpz_inits(P, Q, T, NULL);

  compute_bs(0, iterations, P, Q, T);

  mpfr_t C, sqrtC, T_f, Q_f;
  mpfr_inits(C, sqrtC, T_f, Q_f, NULL);

  // sqrtC = sqrt(10005)
  mpfr_set_ui(sqrtC, E, MPFR_RNDN);
  mpfr_sqrt(sqrtC, sqrtC, MPFR_RNDN);

  // C = 426880 * sqrt(10005)
  mpfr_set_ui(C, D, MPFR_RNDN);
  mpfr_mul(C, C, sqrtC, MPFR_RNDN);

  // Convert big integers to floating point
  mpfr_set_z(T_f, T, MPFR_RNDN); // T → mpfr
  mpfr_set_z(Q_f, Q, MPFR_RNDN); // Q → mpfr

  // pi = (C * Q) / T
  mpfr_mul(Q_f, Q_f, C, MPFR_RNDN);  // Q = Q * C
  mpfr_div(pi, Q_f, T_f, MPFR_RNDN); // pi = Q / T

  // Cleanup
  mpz_clears(P, Q, T, NULL);
  mpfr_clears(C, sqrtC, T_f, Q_f, NULL);
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
