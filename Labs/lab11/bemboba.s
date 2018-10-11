/*
#include <stdio.h>
int addl (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++)
    s += a[i];
  return s;
}

int bemboba (int num) {
  int local[4];
  int *a;
  int i;

  for (i=0,a=local;i<4;i++) {
    *a = num;
    a++;
  }
  return addl (local, 4);
}

int main (void) {
  printf ("%d\n", bemboba (5));
  return 0;
}
*/

.text
.globl bemboba

bemboba:
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  
  movl $0, %r10d  /* i */
  leaq -16(%rbp), %r12  /* a */
  movq %r12, %r11  /* a */
  
for:
  cmpl $4, %r10d
  je fim
  
  movl %edi, (%r11)
  
  addl $1, %r10d
  addq $4, %r11
  jmp for

fim:
   movq   %r12, %rdi
   movl   $4, %esi
   call addl
   	
   leave
   ret
  
