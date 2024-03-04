.section .data
msg:
    .ascii "Hello world\n"
    len = . - msg

.section .text
.globl _start

_start:
    # write
    mov $4, %eax         # write code to eax (4)
    mov $1, %ebx         # stdout fd to ebx (1)
    mov $msg, %ecx       
    mov $len, %edx       
    int $0x80            

    #is_write_correct
    cmp $len, %eax       
    jne write_failed     

    
    xor %ebx, %ebx         
    jmp exit_program     

write_failed:
    mov $-1, %ebx       

exit_program:
    mov $1, %eax         
    int $0x80           
