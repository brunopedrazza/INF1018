#include <stdio.h>
#include <math.h>

#define getsig(x)        ((x)>>31 & 1)
#define getexp(x)        ((x)>>23 & 0xff)
#define getfrac(x)       ((x) & 0x7fffff)
#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))
#define bias 127

typedef union {
  float f;
  unsigned int i;
} U;

float int2float(int i);

float float2(float f);

int main() {
   int i; 
   float f;
   printf("\n******** float2 ****************\n");
   f = 0;
   printf("2 * %f -> %f\n", f, float2(f));
   f = 2.125;
   printf("2 * %f -> %f\n", f, float2(f));
   f = 3.333;
   printf("2 * %f -> %f\n", f, float2(f));
   f = 764.127;
   printf("2 * %f -> %f\n", f, float2(f));
   f = 45.8;
   printf("2 * %f -> %f\n", f, float2(f));
   

   printf("\n******** int2float ****************\n");
   i = 100;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 0;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 1;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -1;  
   printf(" %d -> %10.4f\n", i, int2float(i));
   i = 0x7FFFFFFF;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -i;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   return 0;
}

float float2(float f){
	unsigned int u1, x;
	U u;
	if (!f) return 0;
	u.f = f;
	u1 = u.i;
	x = u1;
	x = getexp(x);
	x += 1;
	u1 = u1 | ((x)<<23);
	u.i = u1;
	f = u.f;
	return f;
}

float int2float(int i){
	U u;
	unsigned int s, e, f;
	e = 31;
	if (!i) return 0;
	s = getsig(i);
	if (s) i *= -1;
	while ( (i & (1<<e)) != (1<<e)) e--;
	if (e > 23) f = i>>(e - 23);
	else f = i<<(23 - e);
	e += bias;
	u.i = makefloat(s,e,f);
	return u.f;
}


