.section .text
# segtree()
L0:
    push %rbp
    mov %rsp, %rbp
    add $0, %rsp
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
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
    # for loop start
    # initialize local variable : int i
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
    # done initialize local variable : int i
L16:
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov $2, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L17
    mov $0, %rax
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
    incq (%rcx)
    jmp L16
L17:
    # for loop end
    add $8, %rsp
    add $0, %rsp
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $8, %rax
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
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
    # for loop start
    # initialize local variable : int i
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
    # done initialize local variable : int i
L18:
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    lea 0(%rax), %rcx
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L19
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
    incq (%rcx)
    jmp L18
L19:
    # for loop end
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret

# segtree::query(int, int)
L3:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int ans
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
L20:
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
    je L21
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
    jne L22
    jmp L23
L22:
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
    add $0, %rsp
    jmp L23
L23:
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
    jne L24
    jmp L25
L24:
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
    add $0, %rsp
    jmp L25
L25:
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
    add $0, %rsp
    jmp L20
L21:
    # for loop end
    add $0, %rsp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -16(%rbp), %rax
    add $8, %rsp
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
L26:
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
    je L27
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
    add $0, %rsp
    jmp L26
L27:
    # while loop end
    add $0, %rsp
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $8, %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
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
    add $0, %rsp
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $8, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
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
    # for loop start
    # initialize local variable : int i
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
    # done initialize local variable : int i
L28:
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
    je L29
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
    jmp L28
L29:
    # for loop end
    add $8, %rsp
    add $0, %rsp
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
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
    # for loop start
    # initialize local variable : int i
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
    # done initialize local variable : int i
L30:
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
    je L31
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
    incq (%rcx)
    jmp L30
L31:
    # for loop end
    add $8, %rsp
    add $0, %rsp
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
    jne L32
    jmp L33
L32:
    add $0, %rsp
    pop %rbp
    ret
    jmp L33
L33:
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
    add $0, %rsp
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
    jne L34
    jmp L35
L34:
    mov 24(%rbp), %rax
    lea 24(%rbp), %rcx
    # calling function : grow
    push %rax
    call L10
    add $0, %rsp
    add $8, %rsp
    jmp L35
L35:
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
    add $0, %rsp
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    # calling function : malloc
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call malloc
    add $8, %rsp
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int* na
    # for loop start
    # initialize local variable : int i
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
    # done initialize local variable : int i
L36:
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
    je L37
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
    add $0, %rsp
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
    jmp L36
L37:
    # for loop end
    add $8, %rsp
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts
    add $8, %rsp
    # calling function : puti_endl
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
    mov %rcx, %rax
    push %rax
    mov -8(%rbp), %rax
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret
    add $0, %rsp

# gen_arr(int)
L12:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int* a
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    # calling function : malloc
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $8, %rax
    push %rax
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call malloc
    add $8, %rsp
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int* a
    # initialize local variable : int mod
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $100007, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int mod
    # initialize local variable : int ptr
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $1, %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int ptr
    # initialize local variable : int mult
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $67, %rax
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int mult
    # initialize local variable : int add
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $103, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int add
    # for loop start
    # initialize local variable : int i
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int i
L38:
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
    je L39
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
    add $0, %rsp
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
    jmp L38
L39:
    # for loop end
    add $8, %rsp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -48(%rbp), %rax
    add $8, %rsp
    add $40, %rsp
    pop %rbp
    ret
    add $40, %rsp

# vector_test()
L13:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : vector a
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
    add $0, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : vector a
    # for loop start
    # initialize local variable : int i
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
    # done initialize local variable : int i
L40:
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
    je L41
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    # calling function : push_back
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
    call L9
    add $8, %rsp
    add $8, %rsp
    add $0, %rsp
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
    jmp L40
L41:
    # for loop end
    add $8, %rsp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $1, %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -16(%rbp), %rax
    add $8, %rsp
    add $8, %rsp
    pop %rbp
    ret
    add $8, %rsp

# segt_test()
L14:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : int n
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $500, %rax
    push %rax
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int n
    # initialize local variable : vector a
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L6
    add $8, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : vector a
    # initialize local variable : segtree b
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
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L1
    add $8, %rsp
    add $8, %rsp
    pop %rax
    push %rax
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : segtree b
    # initialize local variable : int* arr
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    # calling function : gen_arr
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L12
    add $8, %rsp
    push %rax
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int* arr
    # for loop start
    # initialize local variable : int i
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int i
L42:
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
    je L43
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
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    # calling function : at
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L11
    add $8, %rsp
    add $8, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    # calling function : modify
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L4
    add $16, %rsp
    add $8, %rsp
    add $0, %rsp
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
    jmp L42
L43:
    # for loop end
    add $8, %rsp
    add $8, %rsp
    # for loop start
    # initialize local variable : int i
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
    # done initialize local variable : int i
L44:
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
    je L45
    # for loop start
    # initialize local variable : int j
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
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
    # done initialize local variable : int j
L46:
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
    je L47
    # initialize local variable : int aans
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int aans
    # initialize local variable : int bans
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    # calling function : query
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov -72(%rbp), %rax
    lea -72(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    push %rax
    mov -80(%rbp), %rax
    lea -80(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L3
    add $16, %rsp
    add $8, %rsp
    push %rax
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int bans
    # for loop start
    # initialize local variable : int k
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    # done initialize local variable : int k
L48:
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
    je L49
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    # calling function : at
    push %rax
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    push %rax
    mov -88(%rbp), %rax
    lea -88(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call L11
    add $8, %rsp
    add $8, %rsp
    mov %rax, %rcx
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    add $0, %rsp
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
    jmp L48
L49:
    # for loop end
    add $8, %rsp
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
    jne L50
    jmp L51
L50:
    # calling function : puti
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puti
    add $8, %rsp
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
    movb $32, %al
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
    call puts
    add $8, %rsp
    # calling function : puti_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov -56(%rbp), %rax
    lea -56(%rbp), %rcx
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puti_endl
    add $8, %rsp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax
    mov -64(%rbp), %rax
    lea -64(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    mov -64(%rbp), %rax
    add $8, %rsp
    add $56, %rsp
    pop %rbp
    ret
    add $0, %rsp
    jmp L51
L51:
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
    jmp L46
L47:
    # for loop end
    add $8, %rsp
    add $0, %rsp
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
    jmp L44
L45:
    # for loop end
    add $8, %rsp
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
    mov -32(%rbp), %rax
    add $8, %rsp
    add $24, %rsp
    pop %rbp
    ret
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
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
    call L13
    add $0, %rsp
    cmp $0, %rax
    jne L52
    jmp L53
L52:
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    jmp L54
L53:
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    jmp L54
L54:
    # if statement end
    # if statement start
    # calling function : segt_test
    call L14
    add $0, %rsp
    cmp $0, %rax
    jne L55
    jmp L56
L55:
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    jmp L57
L56:
    # calling function : puts_endl
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
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
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    jmp L57
L57:
    # if statement end
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

