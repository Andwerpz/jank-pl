.section .text
.global puts
.global strlen
.global int_to_string

# void puts(string s)
# prints string to stdout
puts:
    push %rbp               # set up new stack frame
    mov %rsp, %rbp

                            # call sys_write(1, string_ptr, strlen(string_ptr))
    mov $1, %rax            # file descriptor : stdout
    push %rax

    mov 16(%rbp), %rax      # buffer to write from
    push %rax

    mov 16(%rbp), %rax      # call strlen to find length of string
    push %rax
    call strlen
    add $8, %rsp
    push %rax

    call sys_write
    add $24, %rsp

    pop %rbp                # return old stack frame
    ret

# int strlen(string s)
# finds the length of a null terminated string
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

# string int_to_string(int x)
# returns a pointer to the created string
int_to_string:
    push %rbp
    mov %rsp, %rbp


    pop %rbp
    ret
