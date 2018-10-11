#include <stdio.h>
#include <math.h>
#include <limits.h>

#define getsig(x)        ((x)>>31 & 1)
#define getexp(x)        ((x)>>23 & 0xff)
#define getfrac(x)       ((x) & 0x7fffff)
#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))
#define bias 127

typedef union {
  float f;
  unsigned int i;
} U; 
   
int float2int(float f);

int main() {
   float f;

   printf("\n******** float2int ****************\n");
   f = 0.0;  
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 1.0;  
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = -1.0;  
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 12345.0;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = -12345.0;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 1.5;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 2.5;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 2.4;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = 0.5;
   printf(" %+10.4f -> %d\n", f, float2int(f));
   f = (float) pow(2,31);
   printf(" %+10.4f (2^31) = overflow -> %d\n", f, float2int(f));
   f = -f;
   printf(" %+10.4f (-2^31) -> %d\n", f, float2int(f));
   f = (float) pow(2,32);
   f = -f;
   printf(" %+10.4f (-2^32 = overflow) -> %d\n", f, float2int(f));

   return 0;
}

int float2int (float f){
   U u;
   unsigned int x, s, e, frac;
   if (f > -1 && f < 1) return 0;
   u.f = f;
   x = u.i;
   e = getexp(x);
   frac = getfrac(x);
   s = getsig(x);
   e -= bias;
   frac = frac>>(23 - e);
   frac = frac | (1<<e);
   if (s) frac = ~(frac) + 1;
   return frac;
}
