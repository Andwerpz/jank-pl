.section .text
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    call prime_checker
    add $0, %rsp
    call while_test
    add $0, %rsp
    push %rax
    call int_to_string
    add $8, %rsp
    push %rax
    call puts_endl
    add $8, %rsp
    mov $0, %rax
    push %rax
    call sys_exit

.global prime_checker
prime_checker:
    push %rbp
    mov %rsp, %rbp
    mov $200, %rax
    push %rax
    mov $2, %rax
    push %rax
L0:
    mov -16(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setle %al
    movzx %al, %rax
    cmp $0, %rax
    je L1
    mov $1, %rax
    push %rax
    mov $2, %rax
    push %rax
L2:
    mov -32(%rbp), %rax
    push %rax
    mov -16(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L3
    mov -16(%rbp), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rdx, %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L4
    jmp L5
L4:
    mov $0, %rax
    mov %rax, -24(%rbp)
    jmp L5
L5:
    mov -32(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -32(%rbp)
    jmp L2
L3:
    add $8, %rsp
    mov -24(%rbp), %rax
    cmp $0, %rax
    jne L6
    jmp L7
L6:
    mov -16(%rbp), %rax
    push %rax
    call int_to_string
    add $8, %rsp
    push %rax
    call puts_endl
    add $8, %rsp
    jmp L7
L7:
    add $8, %rsp
    mov -16(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -16(%rbp)
    jmp L0
L1:
    add $8, %rsp
    add $8, %rsp
    pop %rbp
    ret

.global foo
foo:
    push %rbp
    mov %rsp, %rbp
    mov $123, %rax
    push %rax
    mov $456, %rax
    push %rax
    mov -8(%rbp), %rax
    push %rax
    mov -16(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    push %rax
    mov $10, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    add $16, %rsp
    pop %rbp
    ret
    add $16, %rsp

.global calc
calc:
    push %rbp
    mov %rsp, %rbp
    mov $5, %rax
    push %rax
    mov -8(%rbp), %rax
    push %rax
    mov 16(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    add $8, %rsp
    pop %rbp
    ret
    add $8, %rsp

.global while_test
while_test:
    push %rbp
    mov %rsp, %rbp
    mov $31, %rax
    push %rax
    mov $4, %rax
    push %rax
    mov $0, %rax
    push %rax
L8:
    mov -16(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rdx, %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setne %al
    movzx %al, %rax
    cmp $0, %rax
    je L9
    mov -16(%rbp), %rax
    push %rax
    call int_to_string
    add $8, %rsp
    push %rax
    call puts_endl
    add $8, %rsp
    mov -16(%rbp), %rax
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rdx, %rax
    mov %rax, -16(%rbp)
    mov -24(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -24(%rbp)
    jmp L8
L9:
    mov -24(%rbp), %rax
    add $24, %rsp
    pop %rbp
    ret
    add $24, %rsp

