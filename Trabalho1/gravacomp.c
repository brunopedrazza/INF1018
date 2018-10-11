/* Bruno Pedrazza 1412933 3WB */
/* Guilherme Lacerda 1421134 3WB */

#include <string.h>
#include <stdlib.h>

#include "gravacomp.h"

/* FUNCOES AUXILIARES GRAVACOMP */

static unsigned char montacabi (char ult, int num, unsigned char tam); /* monta cabecalho signed int */
static unsigned char montacabu (char ult, unsigned int num, unsigned char tam); /* monta cabecalho unsigned int */
static unsigned char montacabs (char ult, unsigned char tam); /* monta cabecalho string */

static unsigned char verificaTamu (unsigned int num); /* verifica tamanho necessario para armazenar um unsigned int */
static unsigned char verificaTami (int num); /* verifica tamanho necessario para armazenar um signed int */

static unsigned char verificaPadding (unsigned char tamMax); /* verifica tamanho do padding */

static char ultimo (char *s); /* verifica se é o utimo termo da string descritor */

/* FUNCOES AUXILIARES MOSTRACOMP */

static char verificaTipoeUlt (unsigned char cab, char* ult); /* verifica tipo e se é o ultimo da struct */
static unsigned char verificaNumBytes (unsigned char cab, char tipo); /* verifica numero de bytes necessarios pra representar o tipo */
static unsigned int estendeI (int num, unsigned char numBytes); /* estende um inteiro */

/* GRAVACOMP */

int gravacomp (int nstructs, void* valores, char* descritor, FILE* arquivo){
	int i, j = 0, soma = 0;
	char ult;
	unsigned char aux, tam, tamMax;
	unsigned char *pbyte = (unsigned char *) valores;
	if (fwrite(&nstructs,sizeof(unsigned char),1,arquivo) != 1){ /* imprime nstructs */
		printf("Erro na escrita do arquivo.\n");
		return -1;
	}
	while (nstructs--){  /* tamanho vetor de structs */
		for (i = 0; descritor[i] != '\0'; ){  /* percorre descritor */
			ult = ultimo(&descritor[i]); /* verifica se é o utimo termo da string descritor */
			if (descritor[i] == 'i'){  /* se for signed int */
				int valor = *( (int *)pbyte);
				tam = verificaTami (valor); /* verifica o tamanho de bytes necessario para representar um signed int */
				unsigned char cab = montacabi (ult,valor,tam); /* monta o cabecalho de um signed int */
				if (fwrite(&cab,sizeof(unsigned char),1,arquivo) != 1){ /* imprime cabecalho */
					printf("Erro na escrita do arquivo.\n");
					return -1;
				}
				while (tam){ /* imprime signed int em big endian */
					aux = ((valor)>>(8*(tam-1)));
					tam--;
					if (fwrite(&aux,sizeof(unsigned char),1,arquivo) != 1){
						printf("Erro na escrita do arquivo.\n");
						return -1;
					}
				}
				pbyte += 4; /* percorre 4 bytes na struct */
				i++; /* percorre o descritor */ 
			}
			else if (descritor[i] == 'u'){ /* se for unsigned int */
				unsigned int valor = *( (unsigned int *)pbyte);
				tam = verificaTamu (valor); /* verifica o tamanho de bytes necessario para representar um unsigned int */
				unsigned char cab = montacabu (ult,valor,tam); /* monta o cabecalho de um unsigned int */
				if (fwrite(&cab,sizeof(unsigned char),1,arquivo) != 1){ /* imprime cabecalho */
					printf("Erro na escrita do arquivo.\n");
					return -1;
				}
				while (tam){ /* imprime signed int em big endian */
					aux = ((valor)>>(8*(tam-1)));
					tam--;
					if (fwrite(&aux,sizeof(unsigned char),1,arquivo) != 1){
						printf("Erro na escrita do arquivo.\n");
						return -1;
					}
				}
				pbyte += 4; /* percorre 4 bytes na struct */ 
				i++; /* percorre o descritor */
			}
			else if (descritor[i] == 's'){ /* se for string */
				tamMax = (descritor[i+1] - '0')*10 + (descritor[i+2] - '0'); /* tamanho da string "declarada" na struct */
				tam = strlen((const char*)pbyte); /* tamanho da string ate o '\0'*/
				unsigned char cab = montacabs (ult,tam); /* monta cabecalho string */
				if (fwrite(&cab,sizeof(unsigned char),1,arquivo) != 1){ /* imprime cabecalho string */
					printf("Erro na escrita do arquivo.\n");
					return -1;
				}
				if (fwrite(pbyte,sizeof(unsigned char),tam,arquivo) != tam){ /* imprime cada char da string */
					printf("Erro na escrita do arquivo.\n");
					return -1;
				}
				if (descritor[i-1] == 'i' || descritor[i-1] == 'u'){ /* verifica se existe uma string logo antes dessa */
					while (descritor[i+j] == 's'){ /* soma o tamanho das possiveis proximas strings */
						soma += (descritor[i+j+1] - '0')*10 + (descritor[i+j+2] - '0');
						j += 3;
					}
					j = 0;
				}
				if (descritor[i+3] == 's') /* nao é a ultima da possivel sequencia se strings */
					pbyte += tamMax; /* percorre ate a proxima string na struct */
				else { /* é a ultima string da possivel sequencia de strings */
					pbyte += (verificaPadding(soma) - (soma - tamMax)) ; /* "pula" os paddings */
					soma = 0; /* zera a soma para usos futuros */
				}
				i += 3; /* percorre no descritor, "pulando" os numeros do descritor */
			}
		}
	}
	return 0; /* nao houve erro de gravacao no arquivo */
}


/* MOSTRACOMP */

void mostracomp (FILE* arquivo){
	char str[64];
	unsigned int ui = 0 , uiaux = 0, i;
	char tipo;
	int nstructs;
	unsigned char ult, cab, numBytes, pegaChar;
	nstructs = fgetc(arquivo); /* le do arquivo o primeiro byte que representa o numstructs */
	printf("\nEstruturas:	%d\n\n",nstructs);

	while (nstructs--){  /* tamanho vetor de structs */
		while (ult != 1){  /* verifica se é o ultimo termo da struct */
			cab = fgetc(arquivo); /* pega o cabeçalho */
			ult = verificaTipoeUlt (cab,&tipo); /* pega se é ultimo e o tipo */
			numBytes = verificaNumBytes (cab,tipo); /* pega numero de bytes necessarios pra representar o tipo */
			if (tipo == 'u' || tipo == 'i'){ /* se for unsigned ou signed int */
				for(i = numBytes ; i > 0 ; i--){ /* armazena no ui o valor a ser impresso */
					uiaux = fgetc(arquivo); 
					uiaux = uiaux<<((i-1)*8);
					ui = ui | uiaux;
				}
				if (tipo == 'i'){ /* se for inteiro */
					ui = estendeI (ui,numBytes);  /* estende inteiro se necessario */
					printf ("(int)  %d\t(%08x)\n",ui,ui);  /* imprime inteiro */
				}
				else if (tipo == 'u'){ /* se for unsigned int */
					printf ("(uns)  %u\t(%08x)\n",ui,ui); /* imprime unsigned int */
				}
			}
			else if (tipo == 's'){  /* se for string */

				for (i = 0; i < numBytes ; i++){ /* pega cada char e armazena numa string */
					pegaChar = fgetc(arquivo);
					str[i] = pegaChar;
				}
				str[i] = '\0';
				printf ("(str)  %s\n",str); /* imprime string */
			}
			ui = 0;
		}
		ult = 0; /* prepara ult pra prox struct */
		printf("\n");
	}
}

/* FUNCOES AUXILIARES */

static char verificaTipoeUlt (unsigned char cab, char *tipo){
	if ((cab & (1<<6)) == (1<<6))
		*tipo = 's';
	else {
		if ((cab & (1<<5)) == (1<<5))
			*tipo = 'i';
		else *tipo = 'u';
	}
	if ((cab & (1<<7)) == (1<<7))
		return 1;
	return 0;
}

static unsigned char verificaNumBytes (unsigned char cab, char tipo){
	if (tipo == 's'){
		cab = (cab<<2);
		cab = (cab>>2);
	}
	else{
		cab = (cab<<3);
		cab = (cab>>3);
	}
	return cab;
}

static unsigned int estendeI (int num, unsigned char numBytes){
	unsigned int masc = 0xffffffff;
	if ((num & (1<<((numBytes*8)-1))) == (1<<((numBytes*8)-1))){
		masc = (masc>>((numBytes*8)-1));
		masc = (masc<<((numBytes*8)-1));
		num = masc | num;
	}
	return num;
}



static unsigned char verificaPadding (unsigned char tamMax){
	while (tamMax%4 != 0)
		tamMax++;
	return tamMax;
}


static unsigned char montacabi (char ult, int num, unsigned char tam){
	unsigned char cab = 0;
	cab = cab | tam; 
	if (ult)
		cab = cab | (1<<7);
	cab = cab | (1<<5);
	return cab;
}

static unsigned char montacabu (char ult, unsigned int num, unsigned char tam){
	unsigned char cab = 0;
	cab = cab | tam;
	if (ult)
		cab = cab | (1<<7);   
	return cab;
}

static unsigned char montacabs (char ult, unsigned char tam){
	unsigned char cab = 0;
	if (ult)
		cab = cab | (1<<7);
	cab = cab | (1<<6);
	cab = cab | tam;
	return cab;
}

static unsigned char verificaTamu (unsigned int num){
	char i = 31;
	while (i--){
		if ((num & (1<<i)) == (1<<i))
			break;
	}
	if (i<8)
		return 1;
	else if (i<16)
		return 2;
	else if (i<24)
		return 3;
	return 4;
}

static unsigned char verificaTami (int num){
	char i = 31;
	if ((num & (1<<i)) == (1<<i)){
		while (i--){
			if ((num & (1<<i)) != (1<<i))
				break;
		}
		if (i<7)
			return 1;
		else if (i<15)
			return 2;
		else if (i<23)
			return 3;
		return 4;
	}
	else return verificaTamu (num);
}

static char ultimo (char *s){
	if (s[0] == 'i' || s[0] == 'u')
		return (s[1] == '\0');
	return (s[3] ==  '\0');
}

