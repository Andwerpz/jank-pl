.section .text

# void exit(int status)
.global sys_exit
sys_exit:
    push %rbp
    mov %rsp, %rbp
    mov $60, %rax           
    mov 16(%rbp), %rdi      
    syscall


# ssize_t write(int fd, const void buf, size_t count)
# writes 'count' bytes from 'buf' to 'fd'
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
