#include <stdio.h>

#define getsigdouble(x)        ((x)>>63 & 1)
#define getexpdouble(x)        ((x)>>52 & 0x7ff)
#define getfracdouble(x)       ((x) & 0xfffffffffffff)
#define biasdouble 						 1023

typedef union {
  double d;
  long l;
} U; 

double maxdouble(double a, double b);

int main() {
   double a,b;

   a = 1.0;
   b = 2.0;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = 1.0;
   b = -2.0;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = 1.5;
   b = 1.75;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = -1.5;
   b = -1.75;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   a = 1.5;
   b = -1.75;
   printf("maior de %f %f = %f\n", a,b,maxdouble(a,b));

   return 0;
}

double maxdouble(double a, double b){
	U u;
	char doisnegativos = 0;
	short expa, expb;
	long al, bl;
	u.d = a;
	al = u.l;
	u.d = b;
	bl = u.l;
	if (getsigdouble(al) && !getsigdouble(bl)) return b;
	else if (getsigdouble(bl) && !getsigdouble(al)) return a;
	expa = getexpdouble(al);
	expb = getexpdouble(bl);
	if (getsigdouble(al) && getsigdouble(bl)) doisnegativos = 1;
	if (expa > expb){
		if (doisnegativos) return b;
		else return a;
	}
	else if (expb > expa){
		if (doisnegativos) return a;
		else return b;
	}
	al = getfracdouble(al);
	bl = getfracdouble(bl);
	if (al > bl) {
		if (doisnegativos) return b;
		else return a;
	}
	if (doisnegativos) return a;
	return b;
}

