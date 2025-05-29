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
    # calling function : puts_endl
    mov $19, %rax
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
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $110, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
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
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $110, %al
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
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    # calling function : puts_endl
    mov $53, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $98, %al
    movb %al, (%rbx)
    inc %rbx
    movb $99, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $102, %al
    movb %al, (%rbx)
    inc %rbx
    movb $103, %al
    movb %al, (%rbx)
    inc %rbx
    movb $104, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $106, %al
    movb %al, (%rbx)
    inc %rbx
    movb $107, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $109, %al
    movb %al, (%rbx)
    inc %rbx
    movb $110, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $112, %al
    movb %al, (%rbx)
    inc %rbx
    movb $113, %al
    movb %al, (%rbx)
    inc %rbx
    movb $114, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $117, %al
    movb %al, (%rbx)
    inc %rbx
    movb $118, %al
    movb %al, (%rbx)
    inc %rbx
    movb $119, %al
    movb %al, (%rbx)
    inc %rbx
    movb $120, %al
    movb %al, (%rbx)
    inc %rbx
    movb $121, %al
    movb %al, (%rbx)
    inc %rbx
    movb $122, %al
    movb %al, (%rbx)
    inc %rbx
    movb $65, %al
    movb %al, (%rbx)
    inc %rbx
    movb $66, %al
    movb %al, (%rbx)
    inc %rbx
    movb $67, %al
    movb %al, (%rbx)
    inc %rbx
    movb $68, %al
    movb %al, (%rbx)
    inc %rbx
    movb $69, %al
    movb %al, (%rbx)
    inc %rbx
    movb $70, %al
    movb %al, (%rbx)
    inc %rbx
    movb $71, %al
    movb %al, (%rbx)
    inc %rbx
    movb $72, %al
    movb %al, (%rbx)
    inc %rbx
    movb $73, %al
    movb %al, (%rbx)
    inc %rbx
    movb $74, %al
    movb %al, (%rbx)
    inc %rbx
    movb $75, %al
    movb %al, (%rbx)
    inc %rbx
    movb $76, %al
    movb %al, (%rbx)
    inc %rbx
    movb $77, %al
    movb %al, (%rbx)
    inc %rbx
    movb $78, %al
    movb %al, (%rbx)
    inc %rbx
    movb $79, %al
    movb %al, (%rbx)
    inc %rbx
    movb $80, %al
    movb %al, (%rbx)
    inc %rbx
    movb $81, %al
    movb %al, (%rbx)
    inc %rbx
    movb $82, %al
    movb %al, (%rbx)
    inc %rbx
    movb $83, %al
    movb %al, (%rbx)
    inc %rbx
    movb $84, %al
    movb %al, (%rbx)
    inc %rbx
    movb $85, %al
    movb %al, (%rbx)
    inc %rbx
    movb $86, %al
    movb %al, (%rbx)
    inc %rbx
    movb $87, %al
    movb %al, (%rbx)
    inc %rbx
    movb $88, %al
    movb %al, (%rbx)
    inc %rbx
    movb $89, %al
    movb %al, (%rbx)
    inc %rbx
    movb $90, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    # calling function : puts_endl
    mov $11, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $48, %al
    movb %al, (%rbx)
    inc %rbx
    movb $49, %al
    movb %al, (%rbx)
    inc %rbx
    movb $50, %al
    movb %al, (%rbx)
    inc %rbx
    movb $51, %al
    movb %al, (%rbx)
    inc %rbx
    movb $52, %al
    movb %al, (%rbx)
    inc %rbx
    movb $53, %al
    movb %al, (%rbx)
    inc %rbx
    movb $54, %al
    movb %al, (%rbx)
    inc %rbx
    movb $55, %al
    movb %al, (%rbx)
    inc %rbx
    movb $56, %al
    movb %al, (%rbx)
    inc %rbx
    movb $57, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    # calling function : puts_endl
    mov $30, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $96, %al
    movb %al, (%rbx)
    inc %rbx
    movb $126, %al
    movb %al, (%rbx)
    inc %rbx
    movb $33, %al
    movb %al, (%rbx)
    inc %rbx
    movb $64, %al
    movb %al, (%rbx)
    inc %rbx
    movb $35, %al
    movb %al, (%rbx)
    inc %rbx
    movb $36, %al
    movb %al, (%rbx)
    inc %rbx
    movb $37, %al
    movb %al, (%rbx)
    inc %rbx
    movb $94, %al
    movb %al, (%rbx)
    inc %rbx
    movb $38, %al
    movb %al, (%rbx)
    inc %rbx
    movb $42, %al
    movb %al, (%rbx)
    inc %rbx
    movb $40, %al
    movb %al, (%rbx)
    inc %rbx
    movb $41, %al
    movb %al, (%rbx)
    inc %rbx
    movb $45, %al
    movb %al, (%rbx)
    inc %rbx
    movb $95, %al
    movb %al, (%rbx)
    inc %rbx
    movb $61, %al
    movb %al, (%rbx)
    inc %rbx
    movb $43, %al
    movb %al, (%rbx)
    inc %rbx
    movb $123, %al
    movb %al, (%rbx)
    inc %rbx
    movb $125, %al
    movb %al, (%rbx)
    inc %rbx
    movb $91, %al
    movb %al, (%rbx)
    inc %rbx
    movb $93, %al
    movb %al, (%rbx)
    inc %rbx
    movb $124, %al
    movb %al, (%rbx)
    inc %rbx
    movb $59, %al
    movb %al, (%rbx)
    inc %rbx
    movb $58, %al
    movb %al, (%rbx)
    inc %rbx
    movb $44, %al
    movb %al, (%rbx)
    inc %rbx
    movb $60, %al
    movb %al, (%rbx)
    inc %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $62, %al
    movb %al, (%rbx)
    inc %rbx
    movb $47, %al
    movb %al, (%rbx)
    inc %rbx
    movb $63, %al
    movb %al, (%rbx)
    inc %rbx
    movb $0, %al
    movb %al, (%rbx)
    pop %rax
    push %rax
    call puts_endl
    add $8, %rsp
    # calling function : puts_endl
    mov $50, %rax
    push %rax
    call malloc
    add $8, %rsp
    push %rax
    mov %rax, %rbx
    movb $34, %al
    movb %al, (%rbx)
    inc %rbx
    movb $104, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $121, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $109, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $108, %al
    movb %al, (%rbx)
    inc %rbx
    movb $121, %al
    movb %al, (%rbx)
    inc %rbx
    movb $34, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $118, %al
    movb %al, (%rbx)
    inc %rbx
    movb $101, %al
    movb %al, (%rbx)
    inc %rbx
    movb $110, %al
    movb %al, (%rbx)
    inc %rbx
    movb $46, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $84, %al
    movb %al, (%rbx)
    inc %rbx
    movb $104, %al
    movb %al, (%rbx)
    inc %rbx
    movb $97, %al
    movb %al, (%rbx)
    inc %rbx
    movb $116, %al
    movb %al, (%rbx)
    inc %rbx
    movb $39, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $115, %al
    movb %al, (%rbx)
    inc %rbx
    movb $107, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $98, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $100, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
    movb %al, (%rbx)
    inc %rbx
    movb $32, %al
    movb %al, (%rbx)
    inc %rbx
    movb $111, %al
    movb %al, (%rbx)
    inc %rbx
    movb $104, %al
    movb %al, (%rbx)
    inc %rbx
    movb $105, %al
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
    call puts_endl
    add $8, %rsp
    mov $0, %rax
    add $24, %rsp
    push %rax
    call sys_exit
    add $24, %rsp

