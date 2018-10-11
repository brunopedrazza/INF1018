/* Bruno Pedrazza    1412933 3WB */
/* Guilherme Lacerda 1421134 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cria_func.h"


#define TAM_INICIALIZACAO		4
#define TAM_FINALIZACAO 		2
#define TAM_CALL 						5
#define TAM_SUBQ						4
#define TAM_MAX_PILHA_IDA 	12
#define TAM_MAX_INST				40
#define TAM_MAX_PILHA_VOLTA 12


/*  FUNCOES AUXILIARES  */

/* auxmemcpy ---> automatiza a chamada para memcpy, somando o tamanho e o endereco de novafunc correspondente */
static void* auxmemcpy   (void* novafunc, unsigned char* inst, int taminst, int* tam);

/* auxmemset ---> automatiza a chamada para memset, somando o tamanho e o endereco de novafunc correspondente */
static void* auxmemset   (void* novafunc, unsigned char inst, int* tam);

/* ajustaPilha ---> ajusta a pilha de acordo com a quantidade de PARAM
	 recebe ind = 0 se é uma chamada para "botar" na pilha
	 receve ind = 1 se é uma chamada para "tirar" da pilha
*/
static void* ajustaPilha (void* novafunc, int quant, int* tam, char ind);

/* shiftIntLong ---> faz os shifts correspondentes a um inteiro ou long com o objetivo de passar em little endian para a memoria */
static void shiftIntLong (unsigned long IntLong, unsigned char* inst, int n);


/*  CRIA_FUNC  */
void* cria_func (void* f, DescParam params[], int n){

	unsigned char INICIALIZACAO[] = {0x55,0x48,0x89,0xe5},    /* pushq %rbp /// movq %rsp, %rbp */
								SUBQ[] 					= {0x48,0x83,0xec},         /* subq $___ , %rsp */
								FINALIZACAO[] 	= {0xc9,0xc3};							/* leave ret */

	unsigned char AUXFIX, AUXIND;

	unsigned char PTRFIX[8],
							  INTPTRIND[8],
							  CALL[4],
							  INTFIX[4],
								MOVR10[2];

	
	int contadorPARAM = 0,
			TAM_REAL = 0;
	int i, inteiroFIX, CALLAUX;
	
	unsigned long ponteiroFIX, intptrIND;
	
	OrigemValor origens[n];
	
	int TAM_TOTAL = TAM_INICIALIZACAO 	+       /* soma em TAM_TOTAL a quantidade de bytes de instrucao no "pior" caso */
									TAM_SUBQ						+
									TAM_MAX_PILHA_IDA		+
									TAM_MAX_INST				+
									TAM_CALL						+
									TAM_MAX_PILHA_VOLTA	+
									TAM_FINALIZACAO;
									
	void *novafunc = (void *)malloc(TAM_TOTAL);  /* aloca dinamicamente com esse tamanho do "pior" caso */
	
	novafunc = auxmemcpy (novafunc,INICIALIZACAO,TAM_INICIALIZACAO,&TAM_REAL);  /* copia para a memoria os bytes de instrucao da inicializacao */
	
	for (i = 0 ; i < n ; i++){            /* preenche o array de OrigemValor e soma a quantidade de PARAM */
		if (params[i].orig_val == PARAM){
			contadorPARAM++;
			origens[i] = PARAM;
		}
		else if (params[i].orig_val == FIX) origens[i] = FIX;
		else origens[i] = IND;
	}
	
	if (contadorPARAM > 0){
	
		novafunc = auxmemcpy (novafunc,SUBQ,3,&TAM_REAL);  /* copia para a memoria os bytes de instrucao do subq $___ , %rsp  */

		if (contadorPARAM == 3) novafunc = auxmemset(novafunc,0x20,&TAM_REAL);  /* subq $32 , %rsp */
		else novafunc = auxmemset(novafunc,0x10,&TAM_REAL);											/* subq $16 , %rsp */
		
		novafunc = ajustaPilha (novafunc,contadorPARAM,&TAM_REAL,0);  /* copia para a memoria os bytes de instrucao de chamada para "botar" na pilha  */
		
	  if (n == 3){
			if (contadorPARAM == 2 && origens[2] == PARAM){
				unsigned char MOV[3] = {0x48,0x89,0xf2};  	/*  movq %rsi,%rdx  */
				novafunc = auxmemcpy (novafunc,MOV,3,&TAM_REAL);
				if (origens[0] != PARAM){
					unsigned char MOV[3] = {0x48,0x89,0xfe};  /*  movq %rdi,%rsi  */
					novafunc = auxmemcpy (novafunc,MOV,3,&TAM_REAL);
				}	
			}
	
			if (contadorPARAM == 1 && origens[0] != PARAM){
				unsigned char MOV[2] = {0x48,0x89};   /*  movq %rdi,%___  */
				novafunc = auxmemcpy (novafunc,MOV,2,&TAM_REAL);
		
				if (origens[1] == PARAM) novafunc = auxmemset (novafunc,0xfe,&TAM_REAL); 			/*  movq %rdi, %rsi  */
				else if (origens[2] == PARAM) novafunc = auxmemset (novafunc,0xfa,&TAM_REAL); /*  movq %rdi, %rdx  */
			}
		}
		
		else if (n == 2){
			if (contadorPARAM == 1 && origens[1] == PARAM){
				unsigned char MOV[3] = {0x48,0x89,0xfe};  /*  movq %rdi,%rsi  */
				novafunc = auxmemcpy (novafunc,MOV,3,&TAM_REAL);
			}
		}
	}
	
	for (i=0 ; i<n ; i++){
		if (origens[i] == FIX){
		
			if (i == 0) AUXFIX = 0xbf;      /* mov(l/q) $___,%edi ou %rdi */
			else if (i == 1) AUXFIX = 0xbe;	/* mov(l/q) $___,%esi ou %rsi */
			else if (i == 2) AUXFIX = 0xba;	/* mov(l/q) $___,%edx ou %rdx */
				
			if (params[i].tipo_val == INT_PAR){
				
				novafunc = auxmemset (novafunc,AUXFIX,&TAM_REAL);
				
				inteiroFIX = params[i].valor.v_int;   /* pega na union o valor inteiro */

				shiftIntLong (inteiroFIX,INTFIX,4);   /* faz os shifts necessarios  */
				
				novafunc = auxmemcpy (novafunc,INTFIX,4,&TAM_REAL);  
			}
			else{
				novafunc = auxmemset (novafunc,0x48,&TAM_REAL);   /* movq  $____, %____ */
				
				novafunc = auxmemset (novafunc,AUXFIX,&TAM_REAL); /* %rdi, %rsi ou %rdx */
				
				ponteiroFIX = (unsigned long) params[i].valor.v_ptr;  /* pega na union o endereco */

				shiftIntLong (ponteiroFIX,PTRFIX,8);  /* faz os shifts necessarios  */
				
				novafunc = auxmemcpy (novafunc,PTRFIX,8,&TAM_REAL);
			}
		}
		else if (origens[i] == IND){
			
			MOVR10[0] = 0x49;   /* movq $___, %r10  */
			MOVR10[1] = 0xba;
				
			if (i == 0) AUXIND = 0x3a;      /* %rdi ou %edi */
			else if (i == 1) AUXIND = 0x32; /* %rsi ou %esi */
			else AUXIND = 0x12;							/* %rdx ou %edx */
				
			novafunc = auxmemcpy(novafunc,MOVR10,2,&TAM_REAL);
				
			intptrIND = (unsigned long) params[i].valor.v_ptr;  /* pega na union o endereco */
				
			shiftIntLong (intptrIND,INTPTRIND,8);  /* faz os shifts necessarios  */

			novafunc = auxmemcpy (novafunc,INTPTRIND,8,&TAM_REAL);
				
			if (params[i].tipo_val == INT_PAR) MOVR10[0] = 0x41; /* movl */
			else MOVR10[0] = 0x49;															 /* movq */
				
			MOVR10[1] = 0x8b;  /* mov(l/q) (%r10), %___ */
				
			novafunc = auxmemcpy (novafunc,MOVR10,2,&TAM_REAL);
				
			novafunc = auxmemset (novafunc,AUXIND,&TAM_REAL);
		}
	}
	

	novafunc = auxmemset (novafunc,0xe8,&TAM_REAL); /* copia para a memoria o byte de instrucao relativo a um CALL */
	
	CALLAUX = f - (novafunc + 4);  /* calcula a instrucao para chamar a funcao original corretamente */
	
	shiftIntLong (CALLAUX,CALL,4);  /* faz os shifts necessarios  */
	
	novafunc = auxmemcpy (novafunc,CALL,4,&TAM_REAL); /* copia para a memoria os bytes calculados para chamar a funcao original  */
	
	novafunc = ajustaPilha (novafunc,contadorPARAM,&TAM_REAL,1); /* copia para a memoria os bytes de instrucao de chamada para "tirar" na pilha  */
	
	novafunc = auxmemcpy (novafunc,FINALIZACAO,TAM_FINALIZACAO,&TAM_REAL); /* copia para a memoria os bytes de instrucao da finalizacao */
	
	novafunc -= TAM_REAL;

	return novafunc;
}


/*  LIBERA_FUNC  */
void libera_func (void* func){
	if (func != NULL)
	 free(func);
}


/* FUNCOES AUXILIARES */

static void* auxmemcpy (void* novafunc, unsigned char* inst, int taminst, int* tam){
	memcpy (novafunc,inst,taminst);
	novafunc += taminst;
	(*tam) += taminst;
	
	return novafunc;
}

static void* auxmemset (void* novafunc, unsigned char inst, int* tam){
	memset (novafunc,inst,1);
	novafunc ++;
	(*tam) ++;
	
	return novafunc;
}

static void* ajustaPilha (void* novafunc, int quant, int* tam, char ind){

	unsigned char PILHA_1[]    		= {0x48,0x89,0x7d,0xf8},    /* %rdi, -8(%rbp)  */
								PILHA_2[]    		= {0x48,0x89,0x75,0xf0}, 	  /* %rsi, -16(%rbp) */
								PILHA_3[]    		= {0x48,0x89,0x55,0xe8};		/* %rdx, -24(%rbp) */
								
	if (ind){
		PILHA_1[1] = 0x8b;  /* -8(%rbp), %rdi   */
		PILHA_2[1] = 0x8b;  /* -16(%rbp), %rsi  */
		PILHA_3[1] = 0x8b;  /* -24(%rbp), %rdx  */
	}

	switch (quant){
		case 0: break; /* caso a quant de PARAM seja 0 */
		case 1:				 /* caso a quant de PARAM seja 1 */
			memcpy (novafunc,PILHA_1,4);
			novafunc += 4;
			(*tam) += 4;
			break;
		case 2:				 /* caso a quant de PARAM seja 2 */
			memcpy (novafunc,PILHA_1,4);
			novafunc += 4;
			memcpy (novafunc,PILHA_2,4);
			novafunc += 4;
			(*tam) += 8;
			break;
		case 3:       /* caso a quant de PARAM seja 3 */
			memcpy (novafunc,PILHA_1,4);
			novafunc += 4;
			memcpy (novafunc,PILHA_2,4);
			novafunc += 4;
			memcpy (novafunc,PILHA_3,4);
			novafunc += 4;
			(*tam) += 12;
			break;
	}
	
	return novafunc;
}

static void shiftIntLong (unsigned long IntLong, unsigned char* inst, int n){
	int i;
	if (n == 4) IntLong = (int) IntLong;
	unsigned long masc, auxmasc;
	for (i=0 ; i<n ; i++){
		masc = 0xff;
		masc = masc << 8*i;
		auxmasc = IntLong & masc;
		auxmasc = auxmasc >> 8*i;
		inst[i] = (char)auxmasc;
	}
}

