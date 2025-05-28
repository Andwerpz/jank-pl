.section .data
endl:
    .string "\n"

.section .text

# void puts(char* s)
# prints string to stdout
.global puts
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

# void puts_endl(char* s)
# prints string to stdout with trailing endl
.global puts_endl
puts_endl:
    push %rbp
    mov %rsp, %rbp

    # call puts(s)
    mov 16(%rbp), %rax
    push %rax
    call puts
    add $8, %rsp

    # call puts(endl)
    lea endl(%rip), %rax
    push %rax
    call puts
    add $8, %rsp

    pop %rbp
    ret

# void puti(int x)
# prints int to stdout 
.global puti
puti:
    push %rbp
    mov %rsp, %rbp

    # call int_to_string(x)
    mov 16(%rbp), %rax
    push %rax
    call int_to_string
    add $8, %rsp    

    # call puts(%rax)
    push %rax
    call puts
    add $8, %rsp

    pop %rbp
    ret

# void puti_endl(int x)
# prints int to stdout with trailing endl
.global puti_endl
puti_endl:
    push %rbp
    mov %rsp, %rbp

    # call puti
    mov 16(%rbp), %rax
    push %rax
    call puti
    add $8, %rsp

    # call puts(endl)
    lea endl(%rip), %rax
    push %rax
    call puts
    add $8, %rsp

    pop %rbp
    ret

# int strlen(int s)
# finds the length of a null terminated string
.global strlen
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

# char* int_to_string(int x)
# returns a pointer to the created string
.global int_to_string
int_to_string:
    push %rbp
    mov %rsp, %rbp

    # allocate some space for the string
    mov $20, %rax
    push %rax
    call malloc_char
    add $8, %rsp              
    mov %rax, %rbx          # %rbx points to the start of the buffer
    mov 16(%rbp), %rax      # %rax now stores the integer

    # check if 0
    cmp $0, %rax
    jne 1f

    #just put "0\0"
    movb $'0', 0(%rbx)
    movb $0, 1(%rbx)
    mov %rbx, %rax
    pop %rbp
    ret

1:  #check negative
    mov $0, %r8             # %r8 = is negative
    cmp $0, %rax
    jg 2f
    neg %rax                # make integer positive
    mov $1, %r8

2:  #convert digits
    lea 19(%rbx), %rcx      # %rcx points to end of buffer
    movb $0, (%rcx)
    dec %rcx

3:  #convert loop
    xor %rdx, %rdx
    mov $10, %r9
    div %r9
    add $'0', %dl
    mov %dl, (%rcx)
    dec %rcx
    cmp $0, %rax
    jne 3b

    cmp $0, %r8
    je 4f
    movb $'-', (%rcx)
    dec %rcx

4:  #done
    inc %rcx
    mov %rcx, %rax

    pop %rbp
    ret
