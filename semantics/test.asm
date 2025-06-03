.section .text
# segtree()
L0:
    push %rbp
    mov %rsp, %rbp
    pop %rbp
    ret

# segtree(int)
L1:
    push %rbp
    mov %rsp, %rbp
    pop %rbp
    ret

# vector()
L2:
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
    mov $1, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : malloc
    mov $8, %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    pop %rbp
    ret

# vector(int)
L3:
    push %rbp
    mov %rsp, %rbp
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
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
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : malloc
    mov $8, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L10:
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L11
    mov $0, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L10
L11:
    add $8, %rsp
    # for loop end
    pop %rbp
    ret

# vector::pop_back()
L4:
    push %rbp
    mov %rsp, %rbp
    # if statement start
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L12
    jmp L13
L12:
    add $0, %rsp
    pop %rbp
    ret
    jmp L13
L13:
    # if statement end
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    pop %rbp
    ret

# vector::push_back(int)
L5:
    push %rbp
    mov %rsp, %rbp
    # if statement start
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L14
    jmp L15
L14:
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    # calling function : grow
    push %rax
    call L6
    add $8, %rsp
    jmp L15
L15:
    # if statement end
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    pop %rbp
    ret

# vector::grow()
L6:
    push %rbp
    mov %rsp, %rbp
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # initialize local variable : int* na
    # calling function : malloc
    mov $8, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    # done initialize local variable : int* na
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L16:
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L17
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    push %rcx
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L16
L17:
    add $8, %rsp
    # for loop end
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : puts
    mov $8, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $103, %al
    movb %al, (%rbx)
    inc %rbx
    movb $114, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $119, %al
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
    call puts
    add $8, %rsp
    # calling function : puti_endl
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    call puti_endl
    add $8, %rsp
    add $8, %rsp
    pop %rbp
    ret

# vector::at(int)
L7:
    push %rbp
    mov %rsp, %rbp
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    add $0, %rsp
    pop %rbp
    ret

# vector_test()
L8:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : vector a
    # calling function : vector
    # initialize struct memory vector
    mov $32, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    add $8, %rax
    movq $0, (%rax)
    pop %rbx
    movq %rbx, (%rax)
    # done initialize struct memory vector
    push %rbx
    mov %rbx, %rax
    push %rax
    call L2
    add $8, %rsp
    pop %rax
    push %rax
    # done initialize local variable : vector a
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L18:
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $1000000, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L19
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    # calling function : push_back
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    call L5
    add $16, %rsp
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L18
L19:
    add $8, %rsp
    # for loop end
    add $8, %rsp
    pop %rbp
    ret

# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # calling function : vector_test
    call L8
    add $0, %rsp
    mov $0, %rax
    push %rax
    call sys_exit

