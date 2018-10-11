/*
int novonum(void) {
  int minhalocal;
  printf("numero: ");
  scanf("%d",&minhalocal);
  return minhalocal;
}
*/
.data
Sf: .string "%d"
Sf2: .string "numero:\t"

.text
.globl novonum

novonum:
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  
  movq $Sf2, %rdi
  call printf
 
  movq $Sf, %rdi
  leaq -4(%rbp), %rsi
  call scanf
  
  movl -4(%rbp), %eax
  
  leave
  ret 

