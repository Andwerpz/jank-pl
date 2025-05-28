.section .text
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int N
    mov $10, %rax
    push %rax
    # done initialize local variable : int N
    # initialize local variable : int** dp
    # calling function : malloc
    mov -8(%rbp), %rax
    push %rax
    mov $8, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    # done initialize local variable : int** dp
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L0:
    mov -24(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L1
    # assignment start
    # calling function : malloc
    mov -8(%rbp), %rax
    push %rax
    mov $8, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    lea -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    mov -24(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -24(%rbp)
    # assignment end
    jmp L0
L1:
    add $8, %rsp
    # for loop end
    # assignment start
    mov $1, %rax
    push %rax
    lea -16(%rbp), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    movq %rax, (%rbx)
    # assignment end
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L2:
    mov -24(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L3
    # initialize local variable : int j
    mov $0, %rax
    push %rax
    # done initialize local variable : int j
    # for loop start
L4:
    mov -32(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L5
    # if statement start
    mov -24(%rbp), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setne %al
    movzx %al, %rax
    cmp $0, %rax
    jne L6
    jmp L7
L6:
    # assignment start
    mov -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    lea -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    movq %rax, (%rbx)
    # assignment end
    jmp L7
L7:
    # if statement end
    # if statement start
    mov -32(%rbp), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setne %al
    movzx %al, %rax
    cmp $0, %rax
    jne L8
    jmp L9
L8:
    # assignment start
    mov -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    lea -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    movq %rax, (%rbx)
    # assignment end
    jmp L9
L9:
    # if statement end
    # assignment start
    mov -32(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -32(%rbp)
    # assignment end
    jmp L4
L5:
    add $8, %rsp
    # for loop end
    # assignment start
    mov -24(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -24(%rbp)
    # assignment end
    jmp L2
L3:
    add $8, %rsp
    # for loop end
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L10:
    mov -24(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L11
    # initialize local variable : int j
    mov $0, %rax
    push %rax
    # done initialize local variable : int j
    # for loop start
L12:
    mov -32(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L13
    # calling function : puti
    mov -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    call puti
    add $8, %rsp
    # calling function : puti
    mov $0, %rax
    push %rax
    call puti
    add $8, %rsp
    # assignment start
    mov -32(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -32(%rbp)
    # assignment end
    jmp L12
L13:
    add $8, %rsp
    # for loop end
    # calling function : puti_endl
    mov $0, %rax
    push %rax
    call puti_endl
    add $8, %rsp
    # assignment start
    mov -24(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -24(%rbp)
    # assignment end
    jmp L10
L11:
    add $8, %rsp
    # for loop end
    mov $0, %rax
    add $16, %rsp
    push %rax
    call sys_exit
    add $16, %rsp

