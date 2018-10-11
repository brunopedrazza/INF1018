#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gravacomp.h"

#define nstructs 2

struct teste1 {
	int  i1;
	char s1[5];
	char s2[2];
	unsigned int u1;
	char s3[31];
	unsigned int u2;
	char s4[61];
	char s5[7];
	int i2;
	int i3;
}Teste1[nstructs] = {{-1,"abc","d",99998947,"brunooo",258,"ef","george",40,-32456},
						{-7685,"ABCD","&",432,"brunoguilherme",321,"rewd421","bruno",-54,-1}};

struct teste2 {
	int i;
	char s1[5];
	unsigned int u;
}Teste2[nstructs] = {{-1,"abc",258},{1,"ABCD",65535}};

struct teste3 {
	int i1;
	int i2;
	unsigned int u1;
	int i3;
}Teste3[nstructs] = {{-54,2345,107,-234},{0,5432,234,-798}};

struct teste4 {
	int i1;
	int i2;
	int i3;
	int i4;
	int i5;
	int i6;
	int i7;
}Teste4[nstructs] = {{-55555,-1234,543,7654335,-3,-300,-3212345},
												{3245,-1456234,-980,4323,-646743,12,0}};

struct teste5 {
	char s1[27];
	char s2[18];
	char s3[19];
	char s4[63];
	char s5[31];
}Teste5[nstructs] = {{"Primeiro","trabalho","de","Software","Basico"},
{"fefcyrj[","2w55",",[=-506v","zxcvbnsdfghwertyu987654","23423fwefevwtgtr,;..;,.,m.1"}};


int main (void){
	FILE *arquivo;
	//int i = 0xFF230F87; printf("%d\n",0xFF230F87);
	//unsigned int u = 83745;printf("%u\n",3234);
	//char s[8] = "inf1018";
	arquivo = fopen("arquivo","wb");
	//if (gravacomp (nstructs,&Teste1, "is05s02us31us61s07ii", arquivo) == -1) printf("\n");
	//if (gravacomp (nstructs,&Teste2, "is05u",arquivo) == -1) printf("\n");
	//if (gravacomp (nstructs,&Teste3, "iiui",arquivo) == -1) printf("\n");
	//if (gravacomp (nstructs,&Teste4, "iiiiiii",arquivo) == -1) printf("\n");
	if (gravacomp (nstructs,&Teste5, "s27s18s19s63s31",arquivo) == -1) printf("\n");
	//if (gravacomp (1,&i, "i",arquivo) == -1) printf("\n");
	//if (gravacomp (1,&u, "u",arquivo) == -1) printf("\n");
	//if (gravacomp (1,&s, "s08",arquivo) == -1) printf("\n");
	fclose(arquivo);
	arquivo = fopen("arquivo","rb");
	mostracomp(arquivo);
	fclose(arquivo);
	return 0;
}
