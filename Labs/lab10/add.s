/*
struct X {
  int val;
  struct X *next;
};

int add (struct X *x) {
  int a = 0;
  for (; x != NULL; x = x->next)
    a += x->val;
  return a;
}
*/


.text
.globl add

add:
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)

  movq    %rdi, %r12   
  movl    $0,   %ebx  
  
aqui:
  cmp     $0,   %r12
  je      fim       
 
  addl    (%r12),  %ebx  
  addq    $8,  %r12  

  jmp aqui


fim:
  movl  %ebx, %eax
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  movq    %rbp,  %rsp   
  popq    %rbp  
  ret   
 
     




