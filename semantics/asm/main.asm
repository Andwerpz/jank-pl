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

    mov $0, %rax
    push %rax
    call sys_exit
    