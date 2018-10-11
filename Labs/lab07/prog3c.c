#include <stdio.h>
#define min 1
#define max 10

int main (void){
   int i = min;
   while (i < max + 1){
      printf("%d\n",i*i);
      i++;
   }
   return 0;
}
