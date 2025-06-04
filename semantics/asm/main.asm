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
    mov $2, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # calling function : malloc
    mov $8, %rax
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
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
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    pop %rbp
    ret

# segtree(segtree)
L2:
    push %rbp
    mov %rsp, %rbp
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
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
    # calling function : malloc
    mov $8, %rax
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L17:
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $2, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L18
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
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
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
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
    jmp L17
L18:
    add $8, %rsp
    # for loop end
    pop %rbp
    ret

# segtree::query(int, int)
L3:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int ans
    mov $0, %rax
    push %rax
    # done initialize local variable : int ans
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # for loop start
L19:
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L20
    # if statement start
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov $2, %rax
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
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L21
    jmp L22
L21:
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L22
L22:
    # if statement end
    # if statement start
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov $2, %rax
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
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L23
    jmp L24
L23:
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
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
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L24
L24:
    # if statement end
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L19
L20:
    # for loop end
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    add $8, %rsp
    pop %rbp
    ret
    add $8, %rsp

# segtree::modify(int, int)
L4:
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
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
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
    push %rax
    push %rcx
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
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
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # while loop start
L25:
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setg %al
    movzx %al, %rax
    cmp $0, %rax
    je L26
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
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
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L25
L26:
    # while loop end
    pop %rbp
    ret

# vector()
L5:
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
L6:
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
L27:
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
    je L28
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
    jmp L27
L28:
    add $8, %rsp
    # for loop end
    pop %rbp
    ret

# vector(vector)
L7:
    push %rbp
    mov %rsp, %rbp
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
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
    # calling function : malloc
    mov $8, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
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
L29:
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 8(%rax), %rcx
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L30
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
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
    jmp L29
L30:
    add $8, %rsp
    # for loop end
    pop %rbp
    ret

# vector::pop_back()
L8:
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
    jne L31
    jmp L32
L31:
    add $0, %rsp
    pop %rbp
    ret
    jmp L32
L32:
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
L9:
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
    jne L33
    jmp L34
L33:
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    # calling function : grow
    push %rax
    call L10
    add $8, %rsp
    jmp L34
L34:
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
L10:
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
L35:
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
    je L36
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
    jmp L35
L36:
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
L11:
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

# vector::modify(int, int)
L12:
    push %rbp
    mov %rsp, %rbp
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax
    mov 32(%rbp), %rax
    lea 32(%rbp), %rcx
    lea 16(%rax), %rcx
    lea 16(%rax), %rax
    movq (%rax), %rax
    push %rax
    push %rcx
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
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
    pop %rbp
    ret

# gen_arr(int)
L13:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int* a
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
    # done initialize local variable : int* a
    # initialize local variable : int mod
    mov $100007, %rax
    push %rax
    # done initialize local variable : int mod
    # initialize local variable : int ptr
    mov $1, %rax
    push %rax
    # done initialize local variable : int ptr
    # initialize local variable : int mult
    mov $67, %rax
    push %rax
    # done initialize local variable : int mult
    # initialize local variable : int add
    mov $103, %rax
    push %rax
    # done initialize local variable : int add
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L37:
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L38
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rdx, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    push %rcx
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
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
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L37
L38:
    add $8, %rsp
    # for loop end
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    add $40, %rsp
    pop %rbp
    ret
    add $40, %rsp

# vector_test()
L14:
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
    call L5
    add $8, %rsp
    pop %rax
    push %rax
    # done initialize local variable : vector a
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L39:
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
    je L40
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    # calling function : push_back
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax
    call L9
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
    jmp L39
L40:
    add $8, %rsp
    # for loop end
    mov $1, %rax
    add $8, %rsp
    pop %rbp
    ret
    add $8, %rsp

# segt_test()
L15:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int n
    mov $20, %rax
    push %rax
    # done initialize local variable : int n
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    call L6
    add $16, %rsp
    pop %rax
    push %rax
    # done initialize local variable : vector a
    # initialize local variable : segtree b
    # calling function : segtree
    # initialize struct memory segtree
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
    # done initialize struct memory segtree
    push %rbx
    mov %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    call L1
    add $16, %rsp
    pop %rax
    push %rax
    # done initialize local variable : segtree b
    # initialize local variable : int* arr
    # calling function : gen_arr
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    call L13
    add $8, %rsp
    push %rax
    # done initialize local variable : int* arr
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L41:
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L42
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    # calling function : modify
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    push %rcx
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    call L12
    add $24, %rsp
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    # calling function : modify
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    push %rcx
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rax, %rbx
    pop %rcx
    pop %rax
    push %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rcx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    call L4
    add $24, %rsp
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L41
L42:
    add $8, %rsp
    # for loop end
    add $8, %rsp
    # initialize local variable : int i
    mov $0, %rax
    push %rax
    # done initialize local variable : int i
    # for loop start
L43:
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L44
    # initialize local variable : int j
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    # done initialize local variable : int j
    # for loop start
L45:
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setle %al
    movzx %al, %rax
    cmp $0, %rax
    je L46
    # initialize local variable : int aans
    mov $0, %rax
    push %rax
    # done initialize local variable : int aans
    # initialize local variable : int bans
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    # calling function : query
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    call L3
    add $24, %rsp
    push %rax
    # done initialize local variable : int bans
    # initialize local variable : int k
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    # done initialize local variable : int k
    # for loop start
L47:
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L48
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    # calling function : at
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    push %rax
    call L11
    add $16, %rsp
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L47
L48:
    add $8, %rsp
    # for loop end
    # if statement start
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setne %al
    movzx %al, %rax
    cmp $0, %rax
    jne L49
    jmp L50
L49:
    # calling function : puti
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    call puti
    add $8, %rsp
    # calling function : puts
    mov $2, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
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
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    push %rax
    call puti_endl
    add $8, %rsp
    mov $0, %rax
    add $0, %rsp
    pop %rbp
    ret
    jmp L50
L50:
    # if statement end
    add $16, %rsp
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L45
L46:
    add $8, %rsp
    # for loop end
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    jmp L43
L44:
    add $8, %rsp
    # for loop end
    mov $1, %rax
    add $24, %rsp
    pop %rbp
    ret
    # calling function : puts_endl
    mov $15, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $103, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
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
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    add $24, %rsp

# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # if statement start
    # calling function : vector_test
    call L14
    add $0, %rsp
    cmp $0, %rax
    jne L51
    jmp L52
L51:
    # calling function : puts_endl
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
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $114, %al
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
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    jmp L53
L52:
    # calling function : puts_endl
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
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $114, %al
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
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    jmp L53
L53:
    # if statement end
    # if statement start
    # calling function : segt_test
    call L15
    add $0, %rsp
    cmp $0, %rax
    jne L54
    jmp L55
L54:
    # calling function : puts_endl
    mov $15, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $103, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
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
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    jmp L56
L55:
    # calling function : puts_endl
    mov $15, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $103, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
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
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    jmp L56
L56:
    # if statement end
    mov $0, %rax
    push %rax
    call sys_exit

