#include <gmp.h>
#include <stdio.h>

int main() {
  mpz_t result;
  mpz_init(result);

  mpz_fac_ui(result, 100); // 50 factorial

  gmp_printf("100! = %Zd\n", result);

  mpz_clear(result);
  return 0;
}
