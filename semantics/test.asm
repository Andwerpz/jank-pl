.section .text
# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int x
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $1, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int x
    # initialize local variable : int y
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int y
    # initialize local variable : int z
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $3, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int z
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -32(%rbp), %rax
    add $8, %rsp
    add $24, %rsp
    push %rax
    call sys_exit
    add $16, %rsp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -16(%rbp), %rax
    add $8, %rsp
    add $8, %rsp
    push %rax
    call sys_exit
    add $8, %rsp

