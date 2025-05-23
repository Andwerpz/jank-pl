.section .text

# simple bump allocator
# just increases the heap size every time we want to allocate more memory.

# char* malloc(int sz_bytes)
.global malloc_char
malloc_char:
    push %rbp
    mov %rsp, %rbp

    # call sbrk(sz_bytes)
    mov 16(%rbp), %rax
    push %rax
    call sys_sbrk
    add $8, %rsp

    pop %rbp
    ret

# int* malloc(int sz_quads)
.global malloc_int
malloc_int:
    push %rbp
    mov %rsp, %rbp

    # call sbrk(sz_bytes * 8)
    mov 16(%rbp), %rax
    imul $8, %rax
    push %rax
    call sys_sbrk
    add $8, %rsp

    pop %rbp
    ret

    
    