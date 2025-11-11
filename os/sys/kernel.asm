; AUG 13 2025
; Intermediate 32-bit stage that clears VGA and writes a message before kernel.c

[BITS 32]

global _start 
extern kernel_ini
extern __stack_end 
section .text


_start:
   

 
    mov esp, __stack_end
    mov ebp, esp


.done:
    call kernel_ini
    jmp $                       



