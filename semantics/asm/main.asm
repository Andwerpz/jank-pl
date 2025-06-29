.section .text
# assert(int)
L0:
    push %rbp
    mov %rsp, %rbp
    # if statement start
    # load variable x
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    test %rax, %rax
    sete %al
    movzx %al, %rax
    cmp $0, %rax
    jne L2
    jmp L3
L2:
    # calling function : puts_endl
    # function call member variable : v0
    # TMP:0
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $14, %rax
    push %rax    # emit_malloc() : malloc arg
    call malloc
    # emit_malloc() : malloc arg
    add $8, %rsp
    push %rax    # StringLiteral::emit_asm() : string start
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
    pop %rax    # StringLiteral::emit_asm() : string start
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:0
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    # calling function : sys_exit
    # function call member variable : v0
    # TMP:1
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $1, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:1
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call sys_exit
    add $8, %rsp
    add $0, %rsp
    jmp L3
L3:
    # if statement end
    add $0, %rsp
    pop %rbp
    ret

# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp

    # initialize local variable : int a
    # a
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)

    # calling constructor : int
    mov $8, %rax
    push %rax    # emit_malloc() : malloc arg
    call malloc
    # emit_malloc() : malloc arg
    add $8, %rsp
    movq $0, (%rax)

    push %rax
    push %rbx
    push %rcx
    push %rax
    call puti_endl
    add $8, %rsp
    pop %rcx
    pop %rbx
    pop %rax

    push %rax    # ConstructorCall::emit_asm() : target struct return ref %rax
    push %rax    # ConstructorCall::emit_asm() : target struct
    call L4
    add $0, %rsp
    # ConstructorCall::emit_asm() : target struct
    add $8, %rsp
    pop %rax    # ConstructorCall::emit_asm() : target struct return ref %rax

    push %rax
    push %rbx
    push %rcx
    push %rax
    call puti_endl
    add $8, %rsp
    pop %rcx
    pop %rbx
    pop %rax

    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable a
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    # done initialize local variable : int a
    # TMP:2
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, (%rax)
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:2
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    mov -16(%rbp), %rax
    add $8, %rsp
    add $8, %rsp
    push %rax
    call sys_exit
    add $8, %rsp

# int()
L4:
    push %rbp
    mov %rsp, %rbp
    pop %rbp
    ret

