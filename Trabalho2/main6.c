
#include <stdio.h>
#include "cria_func.h"
#include <math.h>

typedef int (*func_ptr) ();

int minha (int x, int y){
	if (y == 0) return 1;
	int i, soma = x;
	for (i=1 ; i<y ; i++) soma *= x;
	return soma;
}
int main (void) {
  DescParam params[2];
  func_ptr f_potencia = NULL;
  int pot;

  params[0].tipo_val = INT_PAR;
  params[0].orig_val = FIX;
  params[0].valor.v_int = 2;
  
  params[1].tipo_val = INT_PAR;
  params[1].orig_val = IND;
  params[1].valor.v_ptr = &pot;

  f_potencia = (func_ptr) cria_func (minha, params, 2);
  for (pot = 0; pot <=  15; pot++) {
     printf("%d ^ %d = %d\n", params[0].valor.v_int, pot, f_potencia());
  }
  libera_func(f_potencia);
  return 0;
}
