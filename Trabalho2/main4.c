/* Bruno Pedrazza 1412933 3WB */
/* Guilherme Lacerda 1421134 3WB */

#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x);

int mult(int x, int y, int z) {
  return (x * y) + z;
}

int main (void) {
  DescParam params[3];
  func_ptr f_mult;
  int i;

  params[0].tipo_val = INT_PAR; /* o segundo parâmetro de mult é int */
  params[0].orig_val = IND;     /* a nova função passa para mult a constante 10 */
  params[0].valor.v_ptr = &i;

  params[1].tipo_val = INT_PAR; /* o segundo parâmetro de mult é int */
  params[1].orig_val = FIX;     /* a nova função passa para mult a constante 10 */
  params[1].valor.v_int = 10;

	params[2].tipo_val = INT_PAR; /* o terceiro parâmetro de memcmp é um inteiro */
  params[2].orig_val = PARAM;
	
  f_mult = (func_ptr) cria_func (mult, params, 3);

  for (i = 1; i <=10; i++) {
    printf("%d\n", f_mult(i)); /* a nova função só recebe um argumento */
  }

  libera_func(f_mult);
  return 0;
}
