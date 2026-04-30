#ifndef PI_H
#define PI_H

#define A 13591409
#define B 545140134
#define J 640320
#define D 426880
#define E 10005
#define C3 10939058860032000

#include <mpfr.h>

void compute_pi(mpfr_t pi, int digits);
void chudnovsky_term(mpfr_t term, int k);
void compute_constant(mpfr_t C);
void set_precision(int digits);

#endif
