.section .text
# vec2()
L0:
    push %rbp
    mov %rsp, %rbp
    # assignment start
    lea 16(%rbp), %rax
    movq (%rax), %rax
    lea 0(%rax), %rax
    push %rax
    mov $0, %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    lea 16(%rbp), %rax
    movq (%rax), %rax
    lea 8(%rax), %rax
    push %rax
    mov $0, %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    pop %rbp
    ret

# vec2(int, int)
L1:
    push %rbp
    mov %rsp, %rbp
    # assignment start
    lea 32(%rbp), %rax
    movq (%rax), %rax
    lea 0(%rax), %rax
    push %rax
    mov 24(%rbp), %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    lea 32(%rbp), %rax
    movq (%rax), %rax
    lea 8(%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    pop %rbp
    ret

# vec2(vec2)
L2:
    push %rbp
    mov %rsp, %rbp
    # assignment start
    lea 24(%rbp), %rax
    movq (%rax), %rax
    lea 0(%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 0(%rax), %rax
    movq (%rax), %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    lea 24(%rbp), %rax
    movq (%rax), %rax
    lea 8(%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 8(%rax), %rax
    movq (%rax), %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    pop %rbp
    ret

# vec2::add(vec2)
L3:
    push %rbp
    mov %rsp, %rbp
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
    mov 24(%rbp), %rax
    push %rax
    call L2
    add $16, %rsp
    pop %rax
    # calling function : addi
    push %rax
    mov 16(%rbp), %rax
    push %rax
    call L4
    add $16, %rsp
    add $0, %rsp
    pop %rbp
    ret

# vec2::addi(vec2)
L4:
    push %rbp
    mov %rsp, %rbp
    # assignment start
    lea 24(%rbp), %rax
    movq (%rax), %rax
    lea 0(%rax), %rax
    push %rax
    mov 24(%rbp), %rax
    lea 0(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 0(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    # assignment start
    lea 24(%rbp), %rax
    movq (%rax), %rax
    lea 8(%rax), %rax
    push %rax
    mov 24(%rbp), %rax
    lea 8(%rax), %rax
    movq (%rax), %rax
    push %rax
    mov 16(%rbp), %rax
    lea 8(%rax), %rax
    movq (%rax), %rax
    mov %rax, %rbx
    pop %rax
    add %rbx, %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    mov 24(%rbp), %rax
    add $0, %rsp
    pop %rbp
    ret

# vec2::print()
L5:
    push %rbp
    mov %rsp, %rbp
    # calling function : puts
    mov $8, %rax
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
    # calling function : puti
    mov 16(%rbp), %rax
    lea 0(%rax), %rax
    movq (%rax), %rax
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
    # calling function : puti
    mov 16(%rbp), %rax
    lea 8(%rax), %rax
    movq (%rax), %rax
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
    movb $10, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts
    add $8, %rsp
    pop %rbp
    ret

# test1()
L6:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : vec2* a
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
    # done initialize local variable : vec2* a
    # assignment start
    lea -8(%rbp), %rax
    push %rax
    mov $0, %rax
    mov %rax, %rbx
    pop %rax
    mov (%rax), %rax
    lea (%rax, %rbx, 8), %rax
    push %rax
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
    mov $10, %rax
    push %rax
    mov $20, %rax
    push %rax
    call L1
    add $24, %rsp
    pop %rax
    pop %rbx
    movq %rax, (%rbx)
    # assignment end
    mov -8(%rbp), %rax
    movq (%rax), %rax
    # calling function : print
    push %rax
    call L5
    add $8, %rsp
    add $8, %rsp
    pop %rbp
    ret

# test2()
L7:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : vec2 a
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
    mov $1, %rax
    push %rax
    mov $2, %rax
    push %rax
    call L1
    add $24, %rsp
    pop %rax
    push %rax
    # done initialize local variable : vec2 a
    # initialize local variable : vec2 b
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
    mov $3, %rax
    push %rax
    mov $4, %rax
    push %rax
    call L1
    add $24, %rsp
    pop %rax
    push %rax
    # done initialize local variable : vec2 b
    # initialize local variable : vec2 c
    mov -8(%rbp), %rax
    # calling function : add
    push %rax
    mov -16(%rbp), %rax
    push %rax
    call L3
    add $16, %rsp
    push %rax
    # done initialize local variable : vec2 c
    # initialize local variable : vec2 d
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
    mov $1, %rax
    push %rax
    mov $1, %rax
    push %rax
    call L1
    add $24, %rsp
    pop %rax
    # calling function : addi
    push %rax
    mov -16(%rbp), %rax
    push %rax
    call L4
    add $16, %rsp
    # calling function : addi
    push %rax
    mov -24(%rbp), %rax
    push %rax
    call L4
    add $16, %rsp
    push %rax
    # done initialize local variable : vec2 d
    mov -8(%rbp), %rax
    # calling function : print
    push %rax
    call L5
    add $8, %rsp
    mov -16(%rbp), %rax
    # calling function : print
    push %rax
    call L5
    add $8, %rsp
    mov -24(%rbp), %rax
    # calling function : print
    push %rax
    call L5
    add $8, %rsp
    mov -32(%rbp), %rax
    # calling function : print
    push %rax
    call L5
    add $8, %rsp
    add $32, %rsp
    pop %rbp
    ret

# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp
    # calling function : test1
    call L6
    add $0, %rsp
    # calling function : test2
    call L7
    add $0, %rsp
    mov $0, %rax
    push %rax
    call sys_exit

