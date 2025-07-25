.section .text

# void exit(int status)
.global sys_exit
sys_exit:
    push %rbp
    mov %rsp, %rbp
    mov $60, %rax           
    mov 16(%rbp), %rdi      
    syscall

# int write(int fd, void* buf, int count)
# writes 'count' bytes from 'buf' to 'fd'
# on success, returns the number of bytes written
# on failure, returns -1
.global sys_write
sys_write:
    push %rbp
    mov %rsp, %rbp
    mov $1, %rax            
    mov 32(%rbp), %rdi      
    mov 24(%rbp), %rsi      
    mov 16(%rbp), %rdx
    syscall
    pop %rbp
    ret

# int read(int fd, void* buf, int count)
# tries to read 'count' bytes from 'fd' to 'buf'. 
# returns the actual amount of bytes read and increments the file pointer
.global sys_read
sys_read:
    push %rbp
    mov %rsp, %rbp
    mov $0, %rax
    mov 32(%rbp), %rdi
    mov 24(%rbp), %rsi
    mov 16(%rbp), %rdx
    syscall
    pop %rbp
    ret

# int brk(int addr)
# sets the end of the data segment to the address specified
# brk(0) returns the current end of the data segment
.global sys_brk
sys_brk:
    push %rbp
    mov %rsp, %rbp
    mov $12, %rax
    mov 16(%rbp), %rdi
    syscall
    pop %rbp
    ret

# int sbrk(int increment) 
# increases the data space by 'increment' bytes. Returns the old break address
# Not actually a syscall, just a reimplementation of sbrk from libc
.global sys_sbrk
sys_sbrk:
    push %rbp
    mov %rsp, %rbp

    # find current break using sys_brk(0)
    mov $0, %rax
    push %rax
    call sys_brk
    add $8, %rsp

    # save current brk to stack
    push %rax

    # call sys_brk(increment + brk(0))
    add 16(%rbp), %rax
    push %rax
    call sys_brk
    add $8, %rsp

    # %rax = old brk
    pop %rax

    pop %rbp
    ret

