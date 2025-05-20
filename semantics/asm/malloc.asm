.section .text

# int malloc(int sz_bytes)
# simple bump allocator
# just increases the heap size every time we want to allocate more memory.
.global malloc
malloc:
    push %rbp
    mov %rsp, %rbp

    # call sbrk(sz_bytes)
    mov 16(%rbp), %rax
    push %rax
    call sys_sbrk
    add $8, %rsp

    pop %rbp
    ret
    
    