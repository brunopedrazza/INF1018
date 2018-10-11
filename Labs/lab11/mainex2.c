#include <stdio.h>

int bemboba (int num);

int addl (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++)
    s += a[i];
  return s;
}

int main (void) {
  printf ("%d\n", bemboba (10));
  return 0;
}
