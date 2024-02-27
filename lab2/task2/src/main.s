.section .data

msg:
    .ascii "Hello world\n"
    len = . - msg

.section .text

.globl _start

_start:
    #write
    mov $4, %eax         
    mov $1, %ebx         
    mov $msg, %ecx       
    mov $len, %edx       
    int $0x80            
    
    mov %eax, %ebx       

    #exit
    mov $1, %eax         
    int $0x80            
