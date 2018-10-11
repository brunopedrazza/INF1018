#include <stdio.h>

unsigned char switch_byte(unsigned char x){
   unsigned char y, z;
   y = x & (0xF0);
   y = y>>4;
   z = x & (0x0F);
   z = z<<4;
   x = y | z;
   return x;
}

int main (void){
   unsigned char x;
   x = 0xAB;
   x = switch_byte(x);
   printf("%02x\n", x);
   }
   
