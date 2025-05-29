.section .text
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : char* hello_str
    # calling function : malloc
    mov $6, %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    # done initialize local variable : char* hello_str
    # assignment start
    movb $104, %al
    push %rax
    lea -8(%rbp), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # assignment start
    movb $101, %al
    push %rax
    lea -8(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # assignment start
    movb $108, %al
    push %rax
    lea -8(%rbp), %rax
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # assignment start
    movb $108, %al
    push %rax
    lea -8(%rbp), %rax
    push %rax
    mov $3, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # assignment start
    movb $111, %al
    push %rax
    lea -8(%rbp), %rax
    push %rax
    mov $4, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # assignment start
    movb $0, %al
    push %rax
    lea -8(%rbp), %rax
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # initialize local variable : char* space_str
    # calling function : malloc
    mov $2, %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    # done initialize local variable : char* space_str
    # assignment start
    movb $32, %al
    push %rax
    lea -16(%rbp), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # assignment start
    movb $0, %al
    push %rax
    lea -16(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # initialize local variable : char* endl_str
    # calling function : malloc
    mov $2, %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    # done initialize local variable : char* endl_str
    # assignment start
    movb $10, %al
    push %rax
    lea -24(%rbp), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # assignment start
    movb $0, %al
    push %rax
    lea -24(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rbx
    pop %rax
    movb %al, (%rbx)
    # assignment end
    # calling function : puts_endl
    mov -8(%rbp), %rax
    push %rax
    call puts_endl
    add $8, %rsp
    # initialize local variable : int N
    mov $10, %rax
    push %rax
    # done initialize local variable : int N
    # initialize local variable : int** dp
    # calling function : malloc
    mov -32(%rbp), %rax
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
    mov -48(%rbp), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L1
    # assignment start
    # calling function : malloc
    mov -32(%rbp), %rax
    push %rax
    mov $8, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    lea -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    mov -48(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -48(%rbp)
    # assignment end
    jmp L0
L1:
    add $8, %rsp
    # for loop end
    # assignment start
    mov $1, %rax
    push %rax
    lea -40(%rbp), %rax
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
    mov -48(%rbp), %rax
    push %rax
    mov -32(%rbp), %rax
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
    mov -56(%rbp), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L5
    # if statement start
    mov -48(%rbp), %rax
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
    mov -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -56(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -56(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    lea -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    push %rax
    mov -56(%rbp), %rax
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
    mov -56(%rbp), %rax
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
    mov -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -56(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -56(%rbp), %rax
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
    lea -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    push %rax
    mov -56(%rbp), %rax
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
    mov -56(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -56(%rbp)
    # assignment end
    jmp L4
L5:
    add $8, %rsp
    # for loop end
    # assignment start
    mov -48(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -48(%rbp)
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
    mov -48(%rbp), %rax
    push %rax
    mov -32(%rbp), %rax
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
    mov -56(%rbp), %rax
    push %rax
    mov -32(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L13
    # calling function : puti
    mov -40(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -56(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    call puti
    add $8, %rsp
    # calling function : puts
    mov -16(%rbp), %rax
    push %rax
    call puts
    add $8, %rsp
    # assignment start
    mov -56(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -56(%rbp)
    # assignment end
    jmp L12
L13:
    add $8, %rsp
    # for loop end
    # calling function : puts
    mov -24(%rbp), %rax
    push %rax
    call puts
    add $8, %rsp
    # assignment start
    mov -48(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -48(%rbp)
    # assignment end
    jmp L10
L11:
    add $8, %rsp
    # for loop end
    mov $0, %rax
    add $40, %rsp
    push %rax
    call sys_exit
    add $40, %rsp

