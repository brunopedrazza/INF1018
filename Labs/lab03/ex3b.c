#include <stdio.h>

unsigned char rotate_left(unsigned char x, int n){
   unsigned char y;
   y = x;
   y = y >> (8-n);
   x = x << n;
   x = x | y;
   return x;
}

int main (void){
   unsigned char x;
   x = 0x61;
   printf("%02x\n%02x\n%02x\n", rotate_left(x,1), rotate_left(x,2),rotate_left(x,7));
}

