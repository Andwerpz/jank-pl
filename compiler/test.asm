.section .text
# assert(int)
L0:
    push %rbp
    mov %rsp, %rbp
    # if statement start
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    test %rax, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L6
    jmp L7
L6:
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $14, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $114, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $102, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    # calling function : sys_exit
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
    call sys_exit
    add $8, %rsp
    add $0, %rsp
    jmp L7
L7:
    # if statement end
    add $0, %rsp
    pop %rbp
    ret

# basic_tests()
L1:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int a
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $5, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int a
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $3, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $8, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # initialize local variable : int b
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $10, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int b
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $4, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # initialize local variable : int c
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int c
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $12, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # initialize local variable : int d
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $20, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int d
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov $4, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # initialize local variable : int e
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $17, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int e
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rdx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # initialize local variable : int f
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $1, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int f
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov $3, %rax
    mov %rax, %rbx
    pop %rax
    push %rcx
    mov %rbx, %rcx
    sal %cl, %rax
    pop %rcx
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov $8, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # initialize local variable : int g
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $16, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int g
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    push %rcx
    mov %rbx, %rcx
    sar %cl, %rax
    pop %rcx
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    push %rax
    mov $4, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $19, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $98, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $99, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $112, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    add $56, %rsp
    pop %rbp
    ret

# complex_tests()
L2:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int a
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $5, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int a
    # initialize local variable : int b
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $3, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int b
    # initialize local variable : int c
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int c
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L8
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L8:
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    mov $7, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $7, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L10
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $7, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L10:
    cmp $0, %rax
    je L9
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $7, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L9:
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    mov $1, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov $2, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov $3, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # initialize local variable : int d
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int d
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $3, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L12
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L12:
    cmp $0, %rax
    je L11
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L11:
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # initialize local variable : int x
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $10, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int x
    # initialize local variable : int y
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $20, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int y
    # initialize local variable : int z
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int z
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $20, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L14
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov $15, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L14:
    cmp $0, %rax
    je L13
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    push %rax
    mov $35, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L13:
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    mov $1, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov $2, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov $3, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L16
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L16:
    cmp $0, %rax
    je L15
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $3, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L15:
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    mov $1, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov $3, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    push %rcx
    mov %rbx, %rcx
    sal %cl, %rax
    pop %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    or %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $13, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $21, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $99, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $109, %al
    movb %al, (%rbx)
    inc %rbx
    movb $112, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $120, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $112, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    add $56, %rsp
    pop %rbp
    ret

# big_test()
L3:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int a
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
    # done initialize local variable : int a
    # initialize local variable : int b
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
    # done initialize local variable : int b
    # initialize local variable : int c
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
    # done initialize local variable : int c
    # initialize local variable : int d
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $4, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int d
    # initialize local variable : int e
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $5, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int e
    # initialize local variable : int f
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $6, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int f
    # initialize local variable : int g
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $7, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int g
    # initialize local variable : int result
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    push %rcx
    mov %rbx, %rcx
    sal %cl, %rax
    pop %rcx
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    or %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov $4, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rdx, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    and %rbx, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int result
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $3, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    push %rax
    mov $28, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $16, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $98, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $103, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $112, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    add $64, %rsp
    pop %rbp
    ret

# inc_tests()
L4:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int a
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int a
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    incq (%rcx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    decq (%rcx)
    mov %rcx, %rax
    mov %rax, %rcx
    movq (%rax), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    incq (%rcx)
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    add $8, %rsp
    # initialize local variable : int x
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $5, %rax
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    incq (%rcx)
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int y
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L17
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L17:
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    incq (%rcx)
    mov %rcx, %rax
    mov %rax, %rcx
    movq (%rax), %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $7, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L18
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $7, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L18:
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    decq (%rcx)
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $6, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L19
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $7, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L19:
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    decq (%rcx)
    mov %rcx, %rax
    mov %rax, %rcx
    movq (%rax), %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L20
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $5, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    mov %rax, %rbx
    pop %rax
    test %rax, %rax
    setne %al
    movzx %al, %rax
    test %rbx, %rbx
    setne %bl
    movzx %bl, %rbx
    and %rbx, %rax
L20:
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    add $16, %rsp
    # initialize local variable : int a
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $5, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int a
    mov $10, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    incq (%rcx)
    mov %rcx, %rax
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $10, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    add $8, %rsp
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $17, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $110, %al
    movb %al, (%rbx)
    inc %rbx
    movb $99, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $112, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret

# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # calling function : basic_tests
    call L1
    add $0, %rsp
    # calling function : complex_tests
    call L2
    add $0, %rsp
    # calling function : big_test
    call L3
    add $0, %rsp
    # calling function : inc_tests
    call L4
    add $0, %rsp
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $17, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $112, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    add $8, %rsp
    push %rax
    call sys_exit
    add $0, %rsp

