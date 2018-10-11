#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cria_func.h"

typedef int (*func_ptr) ();

int multiplicavetor (int x, int n){
	return x * n;
}

int main (void) {
  DescParam params[2];
  func_ptr multvetor;
	int i;
	
	int** teste = (int **)malloc(5*sizeof(int));
	
	**teste = 2;
	teste += 4;
	**teste = 6;
	teste += 4;
	**teste = 8;
	teste += 4;
	**teste = 20;
	teste += 4;
	**teste = 24;
	teste += 4;
	

  params[0].tipo_val = PTR_PAR;
  params[0].orig_val = IND;
  params[0].valor.v_ptr = teste;
  
  params[1].tipo_val = INT_PAR;
  params[1].orig_val = FIX;
  params[1].valor.v_int = 10;


  multvetor = (func_ptr) cria_func (multiplicavetor, params, 2);

	for (i=0 ; i<5 ; i++){
		
		printf("%d\n",multvetor());
		teste += 4;
	}
  
  libera_func(multvetor);
  
  return 0;
}
