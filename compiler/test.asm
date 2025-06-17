.section .text
# vec2()
L0:
    push %rbp
    mov %rsp, %rbp
    mov $0, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov $0, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    add $0, %rsp
    pop %rbp
    ret

# vec2(vec2&)
L1:
    push %rbp
    mov %rsp, %rbp
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rcx
    movq (%rax), %rax
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rcx
    movq (%rax), %rax
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    add $0, %rsp
    pop %rbp
    ret

# vec2(int, int)
L2:
    push %rbp
    mov %rsp, %rbp
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    add $0, %rsp
    pop %rbp
    ret

# vec2::print()
L3:
    push %rbp
    mov %rsp, %rbp
    # calling function : puts
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $40, %al
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
    call puts
    add $8, %rsp
    # calling function : puti
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puti
    add $8, %rsp
    # calling function : puts
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $3, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $44, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
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
    call puts
    add $8, %rsp
    # calling function : puti
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puti
    add $8, %rsp
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $41, %al
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

# ostream()
L4:
    push %rbp
    mov %rsp, %rbp
    add $0, %rsp
    pop %rbp
    ret

# ostream(ostream&)
L5:
    push %rbp
    mov %rsp, %rbp
    add $0, %rsp
    pop %rbp
    ret

# assert(int)
L6:
    push %rbp
    mov %rsp, %rbp
    # if statement start
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    test %rax, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L18
    jmp L19
L18:
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
    jmp L19
L19:
    # if statement end
    add $0, %rsp
    pop %rbp
    ret

# operator+(vec2, vec2)
L7:
    push %rbp
    mov %rsp, %rbp
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# operator-(vec2, vec2)
L8:
    push %rbp
    mov %rsp, %rbp
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# operator*(vec2, int)
L9:
    push %rbp
    mov %rsp, %rbp
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# operator/(vec2, int)
L10:
    push %rbp
    mov %rsp, %rbp
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# operator==(vec2, vec2)
L11:
    push %rbp
    mov %rsp, %rbp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    je L20
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# operator<<(ostream&, int)
L12:
    push %rbp
    mov %rsp, %rbp
    # calling function : puti
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puti
    add $8, %rsp
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# operator<<(ostream&, char*)
L13:
    push %rbp
    mov %rsp, %rbp
    # calling function : puts
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts
    add $8, %rsp
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# operator<<(ostream&, vec2)
L14:
    push %rbp
    mov %rsp, %rbp
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $40, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L12
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $3, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $44, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L12
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $41, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# vec2_test()
L15:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : vec2 a
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $1, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : vec2 a
    # initialize local variable : vec2 b
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $3, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $4, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : vec2 b
    # initialize local variable : vec2 c
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : operator+
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    call L1
    add $8, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    call L1
    add $8, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L7
    add $16, %rsp
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : vec2 c
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    # calling function : print
    push %rax
    call L3
    add $0, %rsp
    add $8, %rsp
    # calling function : assert
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    # calling function : operator==
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    call L1
    add $8, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $4, %rax
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $6, %rax
    push %rax
    mov -80(%rbp), %rax
    lea -80(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L11
    add $16, %rsp
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L6
    add $8, %rsp
    add $24, %rsp
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
    movb $118, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $99, %al
    movb %al, (%rbx)
    inc %rbx
    movb $50, %al
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

# ostream_test()
L16:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : ostream cout
    # initialize struct memory ostream
    mov $8, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory ostream
    push %rbx
    # calling function : ostream
    # initialize struct memory ostream
    mov $8, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory ostream
    push %rbx
    mov %rbx, %rax
    push %rax
    call L4
    add $0, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : ostream cout
    # calling function : operator<<
    # calling function : operator<<
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $8, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $98, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $33, %al
    movb %al, (%rbx)
    inc %rbx
    movb $33, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $10, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    # initialize local variable : int x
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $100, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int x
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $5, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $88, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $58, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L12
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $10, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    # initialize local variable : vec2 a
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $69, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $420, %rax
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L2
    add $16, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : vec2 a
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $5, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $65, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $58, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    # calling function : vec2
    # initialize struct memory vec2
    mov $24, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vec2
    push %rbx
    mov %rbx, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    call L1
    add $8, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L14
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $10, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    # calling function : operator<<
    # calling function : operator<<
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $16, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $78, %al
    movb %al, (%rbx)
    inc %rbx
    movb $69, %al
    movb %al, (%rbx)
    inc %rbx
    movb $83, %al
    movb %al, (%rbx)
    inc %rbx
    movb $84, %al
    movb %al, (%rbx)
    inc %rbx
    movb $73, %al
    movb %al, (%rbx)
    inc %rbx
    movb $78, %al
    movb %al, (%rbx)
    inc %rbx
    movb $71, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $84, %al
    movb %al, (%rbx)
    inc %rbx
    movb $69, %al
    movb %al, (%rbx)
    inc %rbx
    movb $83, %al
    movb %al, (%rbx)
    inc %rbx
    movb $84, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $58, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $10, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    # calling function : operator<<
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rax
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $10, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L13
    add $16, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $20, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $114, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $109, %al
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
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    add $24, %rsp
    pop %rbp
    ret

# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # calling function : vec2_test
    call L15
    add $0, %rsp
    # calling function : ostream_test
    call L16
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

