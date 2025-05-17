.section .data
msg:
    .ascii "hello\n"

.section .text
.global _start

_start: 
    lea msg(%rip), %rax     # print msg
    push %rax               
    call puts
    pop %rax

    mov     $60, %rax       # syscall: exit
    xor     %rdi, %rdi      # exit code 0
    syscall
    