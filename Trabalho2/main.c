/* Bruno Pedrazza 1412933 3WB */
/* Guilherme Lacerda 1421134 3WB */

#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x, int y);

int mult(int x, int y, int z) {
  return (x * y) + z ;
}

int main (void) {
  DescParam params[3];
  func_ptr f_mult;
  int i, aux1, aux2;


  params[0].tipo_val = INT_PAR;
  params[0].orig_val = PARAM;
  
  params[2].tipo_val = INT_PAR;
  params[2].orig_val = FIX;
  params[2].valor.v_int = 12;
  
  params[1].tipo_val = INT_PAR;
  params[1].orig_val = PARAM;
	
  f_mult = (func_ptr) cria_func (mult, params, 3);

  for (i = 1; i <=10; i++) {
  	aux1 = f_mult(3*i, 10*i);
  	aux2 = mult(3*i,10*i,12);
    printf("%d\t\t%d", aux1, aux2); /* a nova função só recebe um argumento */
    if (aux1 != aux2) printf("\t---> errado\n");
    else printf("\t---> certo\n");
  }

  libera_func(f_mult);
  return 0;
}
