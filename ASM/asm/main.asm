.section .text
# strlen(u8*)
L0:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : u64 ans
    # ans
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0, %rax
    movsxd %eax, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable ans
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    # done initialize local variable : u64 ans
    # for loop start
    # initialize local variable : i32 i
    # i
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable i
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 i
L8:
    movb $0, %al
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable str
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    # load variable i
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    movsxd %eax, %rax
    mov %rax, %rbx
    pop %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rax    # ExprPostfix::emit_asm() : [] %rax 2
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rcx
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 2
    movzbl (%rax, %rbx, 1), %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    cmp %bl, %al
    setne %al
    movzx %al, %rax
    cmp $0, %rax
    je L9
    # load variable ans
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    incq (%rcx)
    add $0, %rsp
    # load variable i
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    incl (%rcx)
    jmp L8
L9:
    # for loop end
    add $8, %rsp
    # TMP:0
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable ans
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:0
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    mov -16(%rbp), %rax
    add $8, %rsp
    add $8, %rsp
    pop %rbp
    ret
    add $8, %rsp

# to_string_bad(f32)
L1:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : u8* str
    # str
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # calling function : malloc
    # function call member variable : v0
    # TMP:1
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $30, %rax
    movsxd %eax, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $1, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    mul %rbx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:1
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call malloc
    add $8, %rsp
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    # done initialize local variable : u8* str
    movb $46, %al
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    mov $15, %rax
    movsxd %eax, %rax
    mov %rax, %rbx
    pop %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rax    # ExprPostfix::emit_asm() : [] %rax 2
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rcx
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 2
    movzbl (%rax, %rbx, 1), %eax
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movb %al, (%rbx)
    # initialize local variable : i32 str_start
    # str_start
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable str_start
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 str_start
    # initialize local variable : i32 a
    # a
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    movd %eax, %xmm0
    cvttss2si %xmm0, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable a
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 a
    # initialize local variable : u8* int_str
    # int_str
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # calling function : int_to_string
    # function call member variable : v0
    # TMP:2
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable a
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:2
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call int_to_string
    add $8, %rsp
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable int_str
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    # done initialize local variable : u8* int_str
    # initialize local variable : u64 len
    # len
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # calling function : strlen
    # function call member variable : str
    # TMP:3
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable int_str
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:3
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call L0
    add $8, %rsp
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable len
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    # done initialize local variable : u64 len
    # load variable len
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %eax, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $15, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    sub %ebx, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable str_start
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # for loop start
    # initialize local variable : i32 i
    # i
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable i
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 i
L10:
    # load variable len
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %eax, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable i
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    cmp %ebx, %eax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L11
    # load variable int_str
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    # load variable i
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    movsxd %eax, %rax
    mov %rax, %rbx
    pop %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rax    # ExprPostfix::emit_asm() : [] %rax 2
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rcx
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 2
    movzbl (%rax, %rbx, 1), %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    # load variable i
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable str_start
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    add %ebx, %eax
    movsxd %eax, %rax
    mov %rax, %rbx
    pop %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rax    # ExprPostfix::emit_asm() : [] %rax 2
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rcx
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 2
    movzbl (%rax, %rbx, 1), %eax
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movb %al, (%rbx)
    add $0, %rsp
    # load variable i
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    incl (%rcx)
    jmp L10
L11:
    # for loop end
    add $8, %rsp
    add $24, %rsp
    # if statement start
    mov $0x00000000, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    ucomiss %xmm1, %xmm0
    setb %al
    movzx %al, %rax
    cmp $0, %rax
    jne L12
    jmp L13
L12:
    mov $0x3f800000, %eax
    xor $0x80000000, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    mulss %xmm1, %xmm0
    movd %xmm0, %eax
    movl %eax, (%rcx)
    jmp L13
L13:
    # if statement end
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    movd %eax, %xmm0
    cvttss2si %xmm0, %eax
    cvtsi2ss %eax, %xmm0
    movd %xmm0, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    subss %xmm1, %xmm0
    movd %xmm0, %eax
    movl %eax, (%rcx)
    # initialize local variable : i32 frac_int
    # frac_int
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0x49742400, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    mulss %xmm1, %xmm0
    movd %xmm0, %eax
    movd %eax, %xmm0
    cvttss2si %xmm0, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable frac_int
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 frac_int
    # for loop start
    # initialize local variable : i32 i
    # i
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable i
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 i
L14:
    mov $6, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable i
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    cmp %ebx, %eax
    setl %al
    movzx %al, %rax
    cmp $0, %rax
    je L15
    mov $10, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable frac_int
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movsx %eax, %rax
    movsx %ebx, %rbx
    cqo
    idiv %rbx
    mov %edx, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    movb $48, %al
    movzbl %al, %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    add %ebx, %eax
    movzbq %al, %rax

    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    # load variable i
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    mov $6, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $15, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    add %ebx, %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    sub %ebx, %eax
    movsxd %eax, %rax
    mov %rax, %rbx
    pop %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rax    # ExprPostfix::emit_asm() : [] %rax 2
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rcx
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 2
    movzbl (%rax, %rbx, 1), %eax
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movb %al, (%rbx)
    mov $10, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable frac_int
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movsx %eax, %rax
    movsx %ebx, %rbx
    cqo
    idiv %rbx
    mov %eax, %eax
    movl %eax, (%rcx)
    add $0, %rsp
    # load variable i
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    incl (%rcx)
    jmp L14
L15:
    # for loop end
    add $8, %rsp
    movb $0, %al
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    mov $1, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $6, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $15, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    add %ebx, %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    add %ebx, %eax
    movsxd %eax, %rax
    mov %rax, %rbx
    pop %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rax    # ExprPostfix::emit_asm() : [] %rax 2
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rcx
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 2
    movzbl (%rax, %rbx, 1), %eax
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movb %al, (%rbx)
    # TMP:4
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    # load variable str_start
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    movsxd %eax, %rax
    mov %rax, %rbx
    pop %rcx    # ExprPostfix::emit_asm() : [] %rcx 1
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 1
    push %rax    # ExprPostfix::emit_asm() : [] %rax 2
    lea (%rax, %rbx, 1), %rax
    mov %rax, %rcx
    pop %rax    # ExprPostfix::emit_asm() : [] %rax 2
    movzbl (%rax, %rbx, 1), %eax
    mov %rcx, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:4
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    mov -32(%rbp), %rax
    add $8, %rsp
    add $24, %rsp
    pop %rbp
    ret
    add $24, %rsp

# print_bits(u32)
L2:
    push %rbp
    mov %rsp, %rbp
    # for loop start
    # initialize local variable : i32 i
    # i
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $31, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable i
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 i
L16:
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable i
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    cmp %ebx, %eax
    setge %al
    movzx %al, %rax
    cmp $0, %rax
    je L17
    # if statement start
    # load variable i
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    mov $1, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    sal %cl, %eax
    pop %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable bits
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    and %rbx, %rax
    cmp $0, %rax
    jne L18
    jmp L19
L18:
    # calling function : puts
    # function call member variable : v0
    # TMP:5
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $2, %rax
    push %rax    # emit_malloc() : malloc arg
    call malloc
    # emit_malloc() : malloc arg
    add $8, %rsp
    push %rax    # StringLiteral::emit_asm() : string start
    mov %rax, %rbx
    movb $49, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax    # StringLiteral::emit_asm() : string start
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:5
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call puts
    add $8, %rsp
    jmp L20
L19:
    # calling function : puts
    # function call member variable : v0
    # TMP:6
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $2, %rax
    push %rax    # emit_malloc() : malloc arg
    call malloc
    # emit_malloc() : malloc arg
    add $8, %rsp
    push %rax    # StringLiteral::emit_asm() : string start
    mov %rax, %rbx
    movb $48, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax    # StringLiteral::emit_asm() : string start
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:6
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call puts
    add $8, %rsp
    jmp L20
L20:
    # if statement end
    add $0, %rsp
    # load variable i
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    decl (%rcx)
    jmp L16
L17:
    # for loop end
    add $8, %rsp
    # calling function : puts_endl
    # function call member variable : v0
    # TMP:7
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $1, %rax
    push %rax    # emit_malloc() : malloc arg
    call malloc
    # emit_malloc() : malloc arg
    add $8, %rsp
    push %rax    # StringLiteral::emit_asm() : string start
    mov %rax, %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax    # StringLiteral::emit_asm() : string start
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:7
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret

# print_bits(f32)
L3:
    push %rbp
    mov %rsp, %rbp
    # calling function : print_bits
    # function call member variable : bits
    # TMP:8
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rcx, %rax
    # dereferencing to type u32
    mov %rax, %rcx
    movl (%rax), %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:8
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L2
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret

# print_bits(i32)
L4:
    push %rbp
    mov %rsp, %rbp
    # calling function : print_bits
    # function call member variable : bits
    # TMP:9
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable x
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:9
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L2
    add $8, %rsp
    add $0, %rsp
    pop %rbp
    ret

# to_string(f32)
L5:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : u32 fbits
    # fbits
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable f
    mov 16(%rbp), %rax
    lea 16(%rbp), %rcx
    mov %rcx, %rax
    # dereferencing to type u32
    mov %rax, %rcx
    movl (%rax), %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable fbits
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : u32 fbits
    # calling function : print_bits
    # function call member variable : bits
    # TMP:10
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable fbits
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:10
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L2
    add $8, %rsp
    # initialize local variable : u32 sign
    # sign
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $31, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $1, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    sal %cl, %eax
    pop %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable fbits
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    and %rbx, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable sign
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : u32 sign
    # initialize local variable : u32 exp
    # exp
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $23, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $1, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $8, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $1, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    sal %cl, %eax
    pop %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    sub %ebx, %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    sal %cl, %eax
    pop %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable fbits
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    and %rbx, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable exp
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : u32 exp
    # initialize local variable : u32 mantissa
    # mantissa
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $1, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $23, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    mov $1, %rax
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    sal %cl, %eax
    pop %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    sub %ebx, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable fbits
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    and %rbx, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable mantissa
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : u32 mantissa
    # calling function : print_bits
    # function call member variable : bits
    # TMP:11
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable sign
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:11
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L2
    add $8, %rsp
    # calling function : print_bits
    # function call member variable : bits
    # TMP:12
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable exp
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:12
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L2
    add $8, %rsp
    # calling function : print_bits
    # function call member variable : bits
    # TMP:13
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable mantissa
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:13
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L2
    add $8, %rsp
    mov $31, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable sign
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    shr %cl, %eax
    pop %rcx
    movl %eax, (%rcx)
    mov $23, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable exp
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    shr %cl, %eax
    pop %rcx
    movl %eax, (%rcx)
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable mantissa
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    push %rcx
    mov %rbx, %rcx
    shr %cl, %eax
    pop %rcx
    movl %eax, (%rcx)
    # initialize local variable : i32 frac_precision
    # frac_precision
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $10, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable frac_precision
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : i32 frac_precision
    # TMP:14
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $5, %rax
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
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $102, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax    # StringLiteral::emit_asm() : string start
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:14
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    mov -48(%rbp), %rax
    add $8, %rsp
    add $40, %rsp
    pop %rbp
    ret
    add $40, %rsp

# root2()
L6:
    push %rbp
    mov %rsp, %rbp
    # initialize local variable : f32 low
    # low
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0x00000000, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable low
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : f32 low
    # initialize local variable : f32 high
    # high
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0x40000000, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable high
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : f32 high
    # initialize local variable : f32 epsilon
    # epsilon
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0x358637bd, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable epsilon
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : f32 epsilon
    # while loop start
L21:
    # load variable epsilon
    mov -24(%rbp), %rax
    lea -24(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable low
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable high
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    subss %xmm1, %xmm0
    movd %xmm0, %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    ucomiss %xmm1, %xmm0
    seta %al
    movzx %al, %rax
    cmp $0, %rax
    je L22
    # initialize local variable : f32 mid
    # mid
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0x40000000, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable low
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable high
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    subss %xmm1, %xmm0
    movd %xmm0, %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    divss %xmm1, %xmm0
    movd %xmm0, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable low
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    addss %xmm1, %xmm0
    movd %xmm0, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable mid
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    # done initialize local variable : f32 mid
    # if statement start
    mov $0x40000000, %eax
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable mid
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable mid
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    mulss %xmm1, %xmm0
    movd %xmm0, %eax
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    ucomiss %xmm1, %xmm0
    seta %al
    movzx %al, %rax
    cmp $0, %rax
    jne L23
    jmp L24
L23:
    # load variable mid
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable high
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    jmp L25
L24:
    # load variable mid
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable low
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    jmp L25
L25:
    # if statement end
    # calling function : puts_endl
    # function call member variable : v0
    # TMP:15
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # calling function : to_string_bad
    # function call member variable : f
    # TMP:16
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable low
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : save right
    # load variable high
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    pop %rbx    # ExprBinary::emit_asm() : save right
    movd %eax, %xmm0
    movd %ebx, %xmm1
    subss %xmm1, %xmm0
    movd %xmm0, %eax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:16
    mov -48(%rbp), %rax
    lea -48(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L1
    add $8, %rsp
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:15
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    # calling function : puti_endl
    # function call member variable : v0
    # TMP:17
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $1, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:17
    mov -40(%rbp), %rax
    lea -40(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call puti_endl
    add $8, %rsp
    add $8, %rsp
    jmp L21
L22:
    # while loop end
    # calling function : print_bits
    # function call member variable : f
    # TMP:18
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable low
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:18
    mov -32(%rbp), %rax
    lea -32(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call L3
    add $8, %rsp
    add $24, %rsp
    pop %rbp
    ret

# main()
.global _start
_start:
    push %rbp
    mov %rsp, %rbp

    # calling function : puts_endl
    # function call member variable : v0
    # TMP:19
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $2, %rax
    push %rax    # emit_malloc() : malloc arg
    call malloc
    # emit_malloc() : malloc arg
    add $8, %rsp
    push %rax    # StringLiteral::emit_asm() : string start
    mov %rax, %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax    # StringLiteral::emit_asm() : string start
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:19
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp
    
    # initialize local variable : u8* int_str
    # int_str
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # calling function : int_to_string
    # function call member variable : v0
    # TMP:20
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $1, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:20
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    call int_to_string
    add $8, %rsp
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable int_str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    # done initialize local variable : u8* int_str

    # calling function : puts_endl
    # function call member variable : v0
    # TMP:21
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    # load variable int_str
    mov -8(%rbp), %rax
    lea -8(%rbp), %rcx
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:21
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movq %rax, (%rbx)
    call puts_endl
    add $8, %rsp

    # TMP:22
    sub $8, %rsp
    lea (%rsp), %rax
    movq $0, %rax
    mov $0, %rax
    push %rax    # ExprBinary::emit_asm() : = save right
    # load variable TMP:22
    mov -16(%rbp), %rax
    lea -16(%rbp), %rcx
    mov %rcx, %rbx
    pop %rax    # ExprBinary::emit_asm() : = save right
    movl %eax, (%rbx)
    mov -16(%rbp), %rax
    add $8, %rsp
    add $8, %rsp
    push %rax
    call sys_exit
    add $8, %rsp

