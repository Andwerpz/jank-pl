.section .text

# void exit(int status)
.global sys_exit
sys_exit:
    push %rbp
    mov %rsp, %rbp
    mov $60, %rax           
    mov 16(%rbp), %rdi      
    syscall

# u64 write(i32 fd, void* buf, u64 count)
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

# u64 read(i32 fd, void* buf, u64 count)
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

# u64 brk(u64 addr)
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

# u64 getrandom(void* buf, u64 buf_len, u64 flags)
# fills the buffer pointed to by buf with up to buf_len random bytes
# apparently on default settings, it's guaranteed that 256 bytes will get written. 
.global sys_getrandom
sys_getrandom:
    push %rbp
    mov %rsp, %rbp
    mov $318, %rax
    mov 32(%rbp), %rdi
    mov 24(%rbp), %rsi
    mov 16(%rbp), %rdx
    syscall
    pop %rbp
    ret

