#include <stdio.h>

void dump (void *p, int n);

int main() {
  char c = 100;
  int i = 10000;
  char v[] = "abcde";

  printf("valor de c: %d na memória: ",c);
  dump(&c, sizeof(c));

  printf("\nvalor de i: %d na memória: ",i);
  dump(&i, sizeof(i));

  printf("\nvalor de v: %s na memória: ",v);
  dump(v, sizeof(v));

  printf("\n");
  return 0;
}

