.section .text
# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int n
    mov $10, %rax
    push %rax
    # done initialize local variable : int n
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L1:
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L2
    # calling function : puti_endl
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    call puti_endl
    add $8, %rsp
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    incq (%rcx)
    jmp L1
L2:
    add $8, %rsp
    # for loop end
    mov $0, %rax
    add $8, %rsp
    push %rax
    call sys_exit
    add $8, %rsp

