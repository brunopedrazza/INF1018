/*
int fat (int n) {
  if (n==0) return 1;
  else return n*fat(n-1);
}
*/

.text
.globl fat
fat:
     pushq %rbp
     movq  %rsp, %rbp
     subq  $16, %rsp
     movq  %r12, -8(%rbp)

     movl %eax, %r12d
     
     cmpl  $0, %r12d
     je L2
     
     movl %r12d, %r10d
     subl $1, %r12d
     imull %r10d, %r12d
     movl %r12d, %edi
     call fat
     
     
L2:
     movl $1, %eax
     movq -8(%rbp), %r12
     leave
     ret
