.section .text
.global puts
.global strlen
.global int_to_string

# prints string to stdout
# - string_ptr
puts:
    push %rbp               # set up new stack frame
    mov %rsp, %rbp

    mov 16(%rbp), %rax      # call strlen to find length of string
    push %rax
    call strlen
    pop %rbx
    mov %rax, %rbx          # put strlen into %rbx

    mov     $1, %rax        # syscall: write
    mov     $1, %rdi        # file descriptor: stdout
    mov     16(%rbp), %rsi  # buffer to write
    mov     %rbx, %rdx      # number of bytes
    syscall

    pop %rbp                # return old stack frame
    ret

# finds the length of a null terminated string
# - string_ptr
strlen:
    push %rbp
    mov %rsp, %rbp

    mov 16(%rbp), %rbx      # put start of string into %rbx
1:                          # increment %rbx until we find null terminator
    cmpb $0, (%rbx)         
    je 2f
    inc %rbx
    jmp 1b
2: 
    sub 16(%rbp), %rbx
    mov %rbx, %rax          # put return value into %rax

    pop %rbp
    ret

# returns a pointer to the created string
# - (int) x
int_to_string:
    
