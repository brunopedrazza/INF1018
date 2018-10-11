/*
int add (int a, int b, int c) {
  return a+b+c;
}
*/

.text
.globl add
add:
     movl $0, %eax
     addl %edi, %eax
     addl %esi, %eax
     addl %edx, %eax
     
     ret
