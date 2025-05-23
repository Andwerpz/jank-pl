.section .text
.global sort
sort:
    push %rbp
    mov %rsp, %rbp
    # if statement start
    mov 16(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L0
    jmp L1
L0:
    add $0, %rsp
    pop %rbp
    ret
    jmp L1
L1:
    # if statement end
    mov 16(%rbp), %rax
    push %rax
    mov $2, %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    # initialize local variable : int lsz
    push %rax
    mov 16(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    sub %rbx, %rax
    # initialize local variable : int rsz
    push %rax
    # calling function : malloc_int
    mov -8(%rbp), %rax
    push %rax
    call malloc_int
    add $8, %rsp
    # initialize local variable : int* l
    push %rax
    # calling function : malloc_int
    mov -16(%rbp), %rax
    push %rax
    call malloc_int
    add $8, %rsp
    # initialize local variable : int* r
    push %rax
    mov $0, %rax
    # initialize local variable : int i
    push %rax
    # for loop start
L2:
    mov -40(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L3
    # assignment start
    mov 24(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    lea -24(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    mov -40(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -40(%rbp)
    # assignment end
    jmp L2
L3:
    add $8, %rsp
    # for loop end
    mov $0, %rax
    # initialize local variable : int i
    push %rax
    # for loop start
L4:
    mov -40(%rbp), %rax
    push %rax
    mov -16(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L5
    # assignment start
    mov 24(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    lea -32(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    mov -40(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -40(%rbp)
    # assignment end
    jmp L4
L5:
    add $8, %rsp
    # for loop end
    # calling function : sort
    mov -24(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    push %rax
    call sort
    add $16, %rsp
    # calling function : sort
    mov -32(%rbp), %rax
    push %rax
    mov -16(%rbp), %rax
    push %rax
    call sort
    add $16, %rsp
    mov $0, %rax
    # initialize local variable : int lptr
    push %rax
    mov $0, %rax
    # initialize local variable : int rptr
    push %rax
    mov $0, %rax
    # initialize local variable : int i
    push %rax
    # for loop start
L6:
    mov -56(%rbp), %rax
    push %rax
    mov 16(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L7
    # if statement start
    mov -48(%rbp), %rax
    push %rax
    mov -16(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L8
    mov -40(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L9
    mov -24(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    mov -32(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    jne L10
    jmp L11
L8:
    # assignment start
    mov -24(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    lea 24(%rbp), %rax
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
    # assignment start
    mov -40(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -40(%rbp)
    # assignment end
    jmp L12
L9:
    # assignment start
    mov -32(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    lea 24(%rbp), %rax
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
    # assignment start
    mov -48(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -48(%rbp)
    # assignment end
    jmp L12
L10:
    # assignment start
    mov -24(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    lea 24(%rbp), %rax
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
    # assignment start
    mov -40(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -40(%rbp)
    # assignment end
    jmp L12
L11:
    # assignment start
    mov -32(%rbp), %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
    push %rax
    lea 24(%rbp), %rax
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
    # assignment start
    mov -48(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -48(%rbp)
    # assignment end
    jmp L12
L12:
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
    jmp L6
L7:
    add $8, %rsp
    # for loop end
    add $48, %rsp
    pop %rbp
    ret

.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    mov $1000, %rax
    # initialize local variable : int N
    push %rax
    # calling function : malloc_int
    mov -8(%rbp), %rax
    push %rax
    call malloc_int
    add $8, %rsp
    # initialize local variable : int* a
    push %rax
    mov $100007, %rax
    # initialize local variable : int mod
    push %rax
    mov $1, %rax
    # initialize local variable : int ptr
    push %rax
    mov $67, %rax
    # initialize local variable : int mult
    push %rax
    mov $103, %rax
    # initialize local variable : int add
    push %rax
    mov $0, %rax
    # initialize local variable : int i
    push %rax
    # for loop start
L13:
    mov -56(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L14
    # assignment start
    mov -32(%rbp), %rax
    push %rax
    mov -40(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    imul %rbx, %rax
    push %rax
    mov -48(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cqo
    idiv %rbx
    mov %rdx, %rax
    mov %rax, -32(%rbp)
    # assignment end
    # assignment start
    mov -32(%rbp), %rax
    push %rax
    lea -16(%rbp), %rax
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
    # assignment start
    mov -56(%rbp), %rax
    push %rax
    mov $1, %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    mov %rax, -56(%rbp)
    # assignment end
    jmp L13
L14:
    add $8, %rsp
    # for loop end
    add $32, %rsp
    # calling function : sort
    mov -16(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    push %rax
    call sort
    add $16, %rsp
    mov $0, %rax
    # initialize local variable : int i
    push %rax
    # for loop start
L15:
    mov -24(%rbp), %rax
    push %rax
    mov -8(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    cmp %rbx, %rax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L16
    # calling function : puti_endl
    mov -16(%rbp), %rax
    push %rax
    mov -24(%rbp), %rax
    mov %rax, %rbx
    pop %rax
    movq (%rax, %rbx, 8), %rax
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
    jmp L15
L16:
    add $8, %rsp
    # for loop end
    mov $0, %rax
    add $16, %rsp
    push %rax
    call sys_exit
    add $16, %rsp

