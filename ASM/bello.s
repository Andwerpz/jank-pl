.section .data
msg:
    .ascii "Bello guys!!!\n"
len = . - msg

.section .text
.global _start
_start:
    mov $1, %rax        # syscall: write
    mov $1, %rdi        # stdout
    mov $msg, %rsi      # pointer to buffer
    mov $len, %rdx      # length of buffer
    syscall

    mov $60, %rax       # syscall: exit
    xor %rdi, %rdi      # exit code 0
    syscall
